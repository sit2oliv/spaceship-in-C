#ifndef OPTIONS_H
#define OPTIONS_H

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

#include <stdio.h> 
#include <stdbool.h>
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef struct spaceship_options
{
  int height;
  int width;
  bool still;
  bool debug;
  bool help;
  bool version;
  bool invalid;
  int difficulty;
  double constant_delay;
  bool pretty;
  int ammo;
  intmax_t bonus;
  intmax_t malus;
} spaceship_options;

////////////////////////////////////////////////////////////////////////////////
// misc.
////////////////////////////////////////////////////////////////////////////////

void print_help(FILE* stream, const char* program);

////////////////////////////////////////////////////////////////////////////////
// init. / destroy etc.
////////////////////////////////////////////////////////////////////////////////

spaceship_options default_options(void);

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void check_options(spaceship_options* o, int argc, char* argv[static argc + 1]);

#endif
