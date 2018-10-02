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
#include <unistd.h> 
#include <locale.h> 
#include <time.h>
#include <sys/time.h>
#include <sysexits.h> 
#include <getopt.h> 

#include "game.h"
#include "options.h"
#include "ui.h"

#define SPACESHIP_INFINITY_VERSION_MAJOR 0
#define SPACESHIP_INFINITY_VERSION_MINOR 0
#define SPACESHIP_INFINITY_VERSION_PATCH 0

static inline void _print_version(void)
{
  printf(
      "spaceship-infinity %u.%u.%u\n",
      SPACESHIP_INFINITY_VERSION_MAJOR,
      SPACESHIP_INFINITY_VERSION_MINOR,
      SPACESHIP_INFINITY_VERSION_PATCH);
  printf("licensed under the WTFPLv2\n");
}

int main(int argc, char* argv[static argc + 1])
{
  spaceship_options o = default_options();
  check_options(&o, argc, argv);

  if (o.help)
  {
    print_help(stdout, argv[0]);
    return EXIT_SUCCESS;
  }
  if (o.version)
  {
    _print_version();
    return EXIT_SUCCESS;
  }
  if (o.invalid)
    return EX_USAGE;

  srandom((unsigned int) (time(NULL) + getpid()));
  setlocale(LC_CTYPE, "");
  interface* const ui = interface_init(o);
  game* const g = game_init(o);

  interface_display(ui, g);
  interface_game_loop(ui, g);

  game_destroy(g);
  interface_destroy(ui);

  return EXIT_SUCCESS;
}
