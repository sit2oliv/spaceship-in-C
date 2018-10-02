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
#include "options.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> 
#include <string.h> 
#include <ctype.h> 

////////////////////////////////////////////////////////////////////////////////
// macros
////////////////////////////////////////////////////////////////////////////////

#ifndef DEFAULT_HEIGHT
  #define DEFAULT_HEIGHT 15
#endif
#ifndef DEFAULT_WIDTH
  #define DEFAULT_WIDTH 30
#endif
#ifndef DEFAULT_PRETTY
  #define DEFAULT_PRETTY true
#endif
#ifndef DEFAULT_DIFFICULTY
  #define DEFAULT_DIFFICULTY 1
#endif
#ifndef DEFAULT_BONUS
  #define DEFAULT_BONUS 1000
#endif
#ifndef DEFAULT_MALUS
  #define DEFAULT_MALUS -1000
#endif

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef enum spaceship_option
{
  OPTION_HELP,
  OPTION_VERSION,
  OPTION_DEBUG,
  OPTION_WIDTH,
  OPTION_HEIGHT,
  OPTION_STILL,
  OPTION_DIFFICULTY,
  OPTION_CONSTANT_DELAY,
  OPTION_PRETTY,
  OPTION_AMMO,
  OPTION_BONUS,
  OPTION_MALUS,
  OPTION_UNKNOWN,
} spaceship_option;

////////////////////////////////////////////////////////////////////////////////
// file-scope variables
////////////////////////////////////////////////////////////////////////////////

static const struct option options[] =
{
  [OPTION_HELP] = { "help", no_argument, 0, 'h', },
  [OPTION_VERSION] = { "version", no_argument, 0, 'v', },
  [OPTION_DEBUG] = { "debug", no_argument, 0, 0, },
  [OPTION_STILL] = { "still", no_argument, 0, 0, },
  [OPTION_WIDTH] = { "width", required_argument, 0, 0, },
  [OPTION_HEIGHT] = { "height", required_argument, 0, 0, },
  [OPTION_DIFFICULTY] = { "difficulty", required_argument, 0, 0, },
  [OPTION_CONSTANT_DELAY] = { "constant-delay", required_argument, 0, 0, },
  [OPTION_PRETTY] = { "pretty", required_argument, 0, 0, },
  [OPTION_AMMO] = { "ammo", required_argument, 0, 0, },
  [OPTION_BONUS] = { "bonus", required_argument, 0, 0, },
  [OPTION_MALUS] = { "malus", required_argument, 0, 0, },
  [OPTION_UNKNOWN] = { 0, 0, 0, 0, },
};

////////////////////////////////////////////////////////////////////////////////
// misc.
////////////////////////////////////////////////////////////////////////////////

void print_help(FILE* const stream, const char* const program)
{
  fprintf(stream, "Usage: %s [option]...\n", program);
  fprintf(stream, "\n");

  fprintf(stream, "Options:\n");
  fprintf(stream, "  -h, --help                Print this help.\n");
  fprintf(stream, "  -v, --version             Print the version.\n");
  fprintf(stream, "  --debug                   Enable debugging mode.\n");
  fprintf(stream, "  --still                   Play in a still world.\n");
  fprintf(stream, "  --width=<value>           Set the width of the world.\n");
  fprintf(stream, "  --height=<value>          Set the height of the world.\n");
  fprintf(stream, "  --difficulty=<value>      Set the difficulty.\n");
  fprintf(stream, "  --pretty=<true|false>     Enable/disable the pretty ui.\n");
  fprintf(stream, "  --constant-delay=<value>  Set a constant delay.\n");
  fprintf(stream, "  --ammo=<value>            Set the ammo amount.\n");
  fprintf(stream, "  --bonus=<value>           Set the bonus value.\n");
  fprintf(stream, "  --malus=<value>           Set the malus value.\n");
}

////////////////////////////////////////////////////////////////////////////////
// local functions declarations
////////////////////////////////////////////////////////////////////////////////

static inline bool _parse_boolean(const char* arg);
static void check_long_options(
    spaceship_options* const o, int option_index, const char* arg);

////////////////////////////////////////////////////////////////////////////////
// init. / destroy etc.
////////////////////////////////////////////////////////////////////////////////

spaceship_options default_options(void)
{
  const spaceship_options o =
  {
    .height = DEFAULT_HEIGHT,
    .width = DEFAULT_WIDTH,
    .still = false,
    .debug = false,
    .help = false,
    .version = false,
    .invalid = false,
    .difficulty = DEFAULT_DIFFICULTY,
    .constant_delay = -1.0,
    .pretty = DEFAULT_PRETTY,
    .ammo = -1,
    .bonus = 1000,
    .malus = -1000,
  };
  return o;
}

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////


void check_options(
    spaceship_options* const o, const int argc,
    char* argv[static const argc + 1])
{
  optind = 0;
  for (;;)
  {
    int option_index = 0;
    const int c = getopt_long_only(argc, argv, ":hv", options, &option_index);
    if (c == -1)
      break;

    switch (c)
    {
      case 0:
        check_long_options(o, option_index, optarg);
        break;
      case 'h':
        o->help = true;
        break;
      case 'v':
        o->version = true;
        break;
      case ':':
        fprintf(stderr, "option '%s' requires an argument\n", argv[optind - 1]);
        o->invalid = true;
        break;
      case '?':
        fprintf(stderr, "unknown option '%s'\n", argv[optind - 1]);
        o->invalid = true;
        break;
      default:
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// local functions definitions
////////////////////////////////////////////////////////////////////////////////

bool _parse_boolean(const char* const arg)
{
  char* const string = strdup(arg);
  const size_t size = strlen(string);
  for (size_t i = 0; i < size; ++i)
    string[i] = (char) tolower(string[i]);

  bool result = false;
  if (!strcmp("true", string) || !strcmp("1", string))
    result = true;
  free(string);

  return result;
}

void check_long_options(
    spaceship_options* const o, const int option_index, const char* const arg)
{
  switch (option_index)
  {
    case OPTION_STILL:
      o->still = true;
      break;
    case OPTION_DEBUG:
      o->debug = true;
      break;
    case OPTION_WIDTH:
      o->width = atoi(arg);
      o->width = o->width > 99 ? 99 : o->width;
      o->width = o->width < 15 ? 15 : o->width;
      break;
    case OPTION_HEIGHT:
      o->height = atoi(arg);
      o->height = o->height > 99 ? 99 : o->height;
      o->height = o->height < 6 ? 6 : o->height;
      break;
    case OPTION_DIFFICULTY:
      o->difficulty = atoi(arg);
      break;
    case OPTION_CONSTANT_DELAY:
      o->constant_delay = atof(arg);
      break;
    case OPTION_PRETTY:
      o->pretty = _parse_boolean(arg);
      break;
    case OPTION_AMMO:
      o->ammo = atoi(arg);
      break;
    case OPTION_BONUS:
      /* We should use strtoimax()... */
      o->bonus = atoi(arg);
      break;
    case OPTION_MALUS:
      /* We should use strtoimax()... */
      o->malus = atoi(arg);
      break;
    default:
      break;
  }
}
