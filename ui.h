#ifndef _UI_H_
#define _UI_H_

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
#include "options.h"

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef struct interface interface;

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

interface* interface_init(spaceship_options o);
void interface_destroy(interface* ui);

////////////////////////////////////////////////////////////////////////////////
// misc.
////////////////////////////////////////////////////////////////////////////////

void interface_display(interface* ui, const game* j);
void interface_game_loop(interface* ui, game* j);
void interface_game_over(interface* ui, spaceship_options o);

#endif
