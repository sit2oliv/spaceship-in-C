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
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <sysexits.h>

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

struct terrain
{
  column_list* columns;
  int height;
  int width;
  int genLow;
  int genHigh;
  int difficulty;
};

////////////////////////////////////////////////////////////////////////////////
// local function declarations
////////////////////////////////////////////////////////////////////////////////

static inline int _trig_low(int genLow, double hmin);
static inline int _trig_high(int genHigh, double height);
static inline column* _trig_column(int genLow, int genHigh, int height);
static inline int _random_generation_selection(int difficulty);
static column* _random_column(int height, int difficulty);
static column* terrain_new_column(terrain* t, bool forward);

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

terrain* terrain_init(const int height, const int width, const int difficulty)
{
  terrain* const t = malloc(sizeof *t);
  if (!t)
  {
    perror("malloc");
    exit(EX_OSERR);
  }


  t->height = height;
  t->width = width;
  t->difficulty = difficulty;
  t->genLow = 0;
  t->genHigh = 0;

  t->columns = column_list_new();
  for (int k = 0; k < width; ++k)
  {
    column* c = NULL;
    if (difficulty != 0 && k > (width - 10))
      c = column_new(height, 0, height - 1);
    else
      c = terrain_new_column(t, true);
    t->columns = column_list_push_front(t->columns, c);
  }

  return t;
}

void terrain_destroy(terrain* const t)
{
  if (!t)
    return;

  if (t->columns)
    column_list_destroy(t->columns);
  free(t);
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

cell terrain_get_cell(const terrain* const t, const size_t x, const size_t y)
{
  const column* c = column_list_get_column(t->columns, x);
  const cell target = column_get_cell(c, y);
  return target;
}

column* terrain_get_column(const terrain* const t, const size_t x)
{
  return column_list_get_column(t->columns, x);
}

column_list* terrain_get_columns(const terrain* const t)
{
  return t->columns;
}

point terrain_start_point(const terrain* const t)
{
  const int x = 1;
  const int height = t->height;

  int y = 0;
  for (int i = 0; i < height; ++i)
    if (terrain_get_cell(t, 1, (size_t) i) == CELL_EMPTY)
    {
      y = i;
      break;
    }

  return (point) { .x = x, .y = y, };
}

int terrain_height(const terrain* const t)
{
  return t->height;
}

int terrain_width(const terrain* const t)
{
  return t->width;
}

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void terrain_right(terrain* const t)
{
  column* const c = terrain_new_column(t, false);
  t->columns = column_list_push_back(t->columns, c);
  t->columns = column_list_pop_front(t->columns);
}

void terrain_fall(terrain* const t)
{
  for (size_t c = 0; c < column_list_get_size(t->columns); ++c)
  {
    column* const current = column_list_get_column(t->columns, c);
    column_fall(current);
  }
}

void terrain_left(terrain* const t)
{
  column* const c = terrain_new_column(t, true);
  t->columns = column_list_push_front(t->columns, c);
  t->columns = column_list_pop_back(t->columns);
}

////////////////////////////////////////////////////////////////////////////////
// local function definitions
////////////////////////////////////////////////////////////////////////////////

int _trig_low(const int genLow, const double hmin)
{
  const double gen = sin(genLow / 3.14) * 6.0;
  return (int) (gen < hmin ? hmin : gen);
}

int _trig_high(const int genHigh, const double height)
{
  const double gen = height - cos(genHigh / 3.14) * 6.0;
  return (int) (gen > height ? height : gen);
}

column* _trig_column(const int genLow, const int genHigh, const int height)
{
  int high = _trig_high(genHigh, height);
  int low = _trig_low(genLow, 0);
  column* c = column_new(height, low, high);

  return c;
}

int _random_generation_selection(const int difficulty)
{
  return difficulty + (random() % 100 < 2 ? 1 : 0);
}

column* _random_column(const int height, const int difficulty)
{
  const int half = height / 2;
  const int selection = _random_generation_selection(difficulty);

  int top = 0;
  int bottom = 0;

  if (selection <= 1)
  {
    const int bias_divisor = 2 + (int) random() % 4;
    const int bias_limit = height / bias_divisor;
    const int bias = (int) (random() % (bias_limit));
    top = (int) (random() % half) + bias;
    bottom = half + (int) (random() % half) - bias;
    top = top > bottom ? bottom - 1 : top;
    top = top < 0 ? 0 : top;
  }
  else
  {
    top = (int) (random() % height);
    if (top > half)
      top -= (int) (random() % half);

    bottom = (int) (random() % (height - top) + top);
    if ((bottom - top) <= 1)
      bottom += (int) (random() % half);
  }

  return column_new(height, top, bottom);
}

column* terrain_new_column(terrain* const t, const bool forward)
{
  const int difficulty = t->difficulty;
  column* c = NULL;
  if (difficulty <= 0)
  {
    t->genLow += forward ? 1 : -1;
    t->genHigh += forward ? 1 : -1;
    c = _trig_column(t->genLow, t->genHigh, t->height);
  }
  else if (difficulty >= 1)
  {
    c = _random_column(t->height, difficulty);
    const int threshold = 10 - difficulty;
    const int threshold_number = (int) random() % 100;
    if (threshold_number < threshold)
    {
      const int selector = (int) random() % 100;
      const size_t y = (size_t) (random() % t->height);

      cell selection = CELL_EMPTY;
      if (selector < 30)
        selection = CELL_SECRET;
      else if (selector < 60)
        selection = CELL_BONUS;
      else if (selector < 80)
        selection = CELL_MALUS;
      else
        selection = CELL_AMMO;
      column_set_cell(c, y, selection);
    }
  }

  return c;
}
