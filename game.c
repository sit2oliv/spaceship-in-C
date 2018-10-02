/*
 *        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */
#include "game.h"

#include <stdio.h>
#include <tgmath.h>
#include <float.h>
#include <sysexits.h>

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

struct game
{
  spaceship_options options;
  terrain* map;
  point ship;
  bool debug;
  double delay;
  double elapsed_time;
  intmax_t bonus;
  int last_key;
  point_list* bullets;
  size_t bullet_max;
};

////////////////////////////////////////////////////////////////////////////////
// local functions declarations
////////////////////////////////////////////////////////////////////////////////

static intmax_t game_get_bonus(const game* g);
static column* game_get_ship_column(const game* g);

static void game_shift_right(game* g);
static void game_move_bullets(game* g);
static void game_fall(game* g);
static void game_check_bullets(game* g);
static void game_check_special_cells(game* g);
static void game_add_bonus(game* g, intmax_t bonus);

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

game* game_init(const spaceship_options options)
{
  const int height = options.height;
  const int w = options.width;
  const int difficulty = options.difficulty;
  const int ammo = options.ammo;

  game* const g = malloc(sizeof *g);
  if (!g)
  {
    perror("malloc");
    exit(EX_OSERR);
  }
  terrain* const map = terrain_init(height, w, difficulty);
  if (!map)
  {
    perror("malloc");
    exit(EX_OSERR);
  }
  /*
   * Select an empty cell for the ship... we don't want the game to be over
   * right away
   */
  const point ship = terrain_start_point(map);

  g->ship = ship;
  g->map = map;
  g->last_key = 0;
  g->debug = options.debug;
  g->options = options;
  g->bonus = 0;
  g->elapsed_time = 0.0;
  if (ammo > 0)
    g->bullet_max = (size_t) ammo;
  else
    g->bullet_max = difficulty < 3 ? 5 - (size_t) difficulty : 1;
  g->bullets = point_list_new();
  g->delay = DBL_MIN;

  return g;
}

void game_destroy(game* const g)
{
  if (!g)
    return;

  if (g->map)
    terrain_destroy(g->map);
  if (g->bullets)
    point_list_destroy(g->bullets);
  free(g);
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

double game_get_delay(const game* const g)
{
  return g->delay;
}

double game_get_elapsed_time(const game* const g)
{
  return g->elapsed_time;
}
intmax_t game_get_score(const game* const g)
{
  const double elapsed = game_get_elapsed_time(g);
  const intmax_t bonus = game_get_bonus(g);
  const intmax_t score = bonus + (intmax_t) elapsed * 10;
  return score;
}

double game_get_constant_delay(const game* const g)
{
  return g->options.constant_delay;
}

spaceship_options game_get_options(const game* const g)
{
  return g->options;
}

terrain* game_get_map(const game* const g)
{
  return g->map;
}

point game_get_ship_position(const game* const g)
{
  return g->ship;
}

size_t game_get_max_ammo(const game* const g)
{
  return g->bullet_max;
}

size_t game_get_fired_bullets(const game* const g)
{
  return point_list_get_size(g->bullets);
}

point_list* game_get_bullets(const game* const g)
{
  return g->bullets;
}

int game_get_last_input(const game* const g)
{
  return g->last_key;
}

bool game_ship_is_alive(const game* const g)
{
  const point ship = g->ship;
  const column* const c = game_get_ship_column(g);
  const cell position = column_get_cell(c, (size_t) ship.y);
  return position != CELL_WALL;
}


////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void game_compute_turn(game* const g)
{
  game_check_special_cells(g);
  game_check_bullets(g);
  game_shift_right(g);
  game_check_bullets(g);
  game_move_bullets(g);
  game_check_bullets(g);
  game_fall(g);
  game_check_bullets(g);
  game_check_special_cells(g);
}

void game_set_delay(game* const g, const double delay)
{
  g->delay = delay;
}

void game_set_elapsed_time(game* const g, const double t)
{
  g->elapsed_time = t;
}

void game_process_input(game* const g, const int key)
{
  const spaceship_options options = g->options;
  const int height = terrain_height(g->map);
  const int difficulty = options.difficulty;

  terrain* const map = g->map;
  const int width = terrain_width(map);
  const size_t fired = point_list_get_size(g->bullets);

  point ship = g->ship;
  const size_t x = (size_t) ship.x;
  const size_t y = (size_t) ship.y;
  switch (key) {
    /* Haut. */
    case '8':
    case 'k':
    case 65:
      if (difficulty >= 2 && key != 'k')
        /* vim mode: move with h, j, k, l! */
        break;
      if (y > 0
          && terrain_get_cell(map, x, y - 1) != CELL_WALL)
        ship.y--;
      break;
    /* Bas. */
    case '2':
    case 'j':
    case 66:
      /* vim mode: move with h, j, k, l! */
      if (difficulty >= 2 && key != 'j')
        break;
      if ((int) y < (height - 1)
          && terrain_get_cell(map, x, y + 1) != CELL_WALL)
        ship.y++;
      break;
    /* Gauche. */
    case '4':
    case 'h':
    case 68:
      /* vim mode: move with h, j, k, l! */
      if (difficulty >= 2 && key != 'h')
        break;
      if (x >= 1 &&
          terrain_get_cell(map, x - 1, y) != CELL_WALL)
        ship.x--;

      if (ship.x <= 0 && g->options.difficulty <= 0)
      {
        /* Si on a atteint le bord gauche. */
        terrain_left(map);
        ship.x++;
        point_list_shift_right(g->bullets);
      }
      break;
    /* Droite. */
    case '6':
    case 'l':
    case 67:
      /* vim mode: move with h, j, k, l! */
      if (difficulty >= 2 && key != 'l')
        break;
      if ((int) x < width
          && terrain_get_cell(map, x + 1, y) != CELL_WALL)
        ship.x++;

      if (ship.x >= width - 1)
      {
        /* Si on a atteint le bord droit. */
        terrain_right(map);
        ship.x--;
        point_list_shift_left(g->bullets);
      }
      break;
    /* Space. */
    case ' ':
      if (fired < g->bullet_max)
      {
        point p = (point) { .x = ship.x + 1, .y = ship.y };
        if (!point_list_contains(g->bullets, p))
        {
          g->bullets = point_list_push_back(g->bullets, p);
          game_add_bonus(g, -20 * (intmax_t) ((fired + 1) * (fired + 1)));
        }
      }
      break;
    default:
      break;
  }

  g->map = map;
  g->ship = ship;
  g->last_key = key;
  game_check_special_cells(g);
}


////////////////////////////////////////////////////////////////////////////////
// local functions definitions
////////////////////////////////////////////////////////////////////////////////

void game_add_bonus(game* const g, const intmax_t bonus)
{
  g->bonus += bonus;
}

intmax_t game_get_bonus(const game* const g)
{
  return g->bonus;
}

void game_move_bullets(game* const g)
{
  point_list_shift_right(g->bullets);
}

void game_shift_right(game* const g)
{
  terrain_right(g->map);
}

void game_check_special_cells(game* const g)
{
  const spaceship_options options = g->options;
  const point ship = g->ship;
  column* const c = game_get_ship_column(g);

  cell position = column_get_cell(c, (size_t) ship.y);
  if (position == CELL_SECRET)
  {
    const int selector = (int) random() % 100;
    if (selector < 25)
      position = CELL_AMMO;
    else if (selector < 50)
      position = CELL_BONUS;
    else if (selector < 75)
      position = CELL_MALUS;
    else
      position = CELL_EMPTY;
  }

  if (position == CELL_AMMO)
  {
    g->bullet_max += g->bullet_max < 10 ? 1 : 0;
    column_set_cell(c, (size_t) ship.y, CELL_EMPTY);
  }
  else if (position == CELL_BONUS)
  {
    game_add_bonus(g, options.bonus);
    column_set_cell(c, (size_t) ship.y, CELL_EMPTY);
  }
  else if (position == CELL_MALUS)
  {
    game_add_bonus(g, options.malus);
    column_set_cell(c, (size_t) ship.y, CELL_EMPTY);
  }
}

column* game_get_ship_column(const game* g)
{
  terrain* const map = g->map;
  const point ship = g->ship;
  const column_list* columns = terrain_get_columns(map);
  column* const c = column_list_get_column(columns, (size_t) ship.x);
  return c;
}

void game_fall(game* const g)
{
  terrain_fall(g->map);
}

void game_check_bullets(game* const g)
{
  const spaceship_options options = g->options;
  const point up_left = { .x = 0, .y = 0, };
  const point bottom_right = { .x = options.width, .y = options.height, };

  const column_list* map = terrain_get_columns(g->map);

  g->bullets = point_list_prune_out_of_bounds(g->bullets, up_left, bottom_right);
  const size_t count = point_list_get_size(g->bullets);
  for (size_t i = 0; i < count; ++i)
  {
    const point position = point_list_get_point(g->bullets, i);
    if (point_is_valid(position))
    {
      column* const c = column_list_get_column(map, (size_t) position.x);
      if (c && column_get_cell(c, (size_t) position.y) != CELL_EMPTY)
      {
        column_set_cell(c, (size_t) position.y, CELL_EMPTY);
        point_list_set_point(g->bullets, i, point_invalid());
      }
    }
  }
  g->bullets = point_list_prune_out_of_bounds(g->bullets, up_left, bottom_right);
}
