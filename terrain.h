#ifndef _TERRAIN_H_
#define _TERRAIN_H_

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

#include "point.h"
#include "column.h"
#include "column_list.h"

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef struct terrain terrain;

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

terrain* terrain_init(int height, int width, int difficulty);
void terrain_destroy(terrain* t);

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

cell terrain_get_cell(const terrain* l, size_t x, size_t y);
column* terrain_get_column(const terrain* l, size_t x);
column_list* terrain_get_columns(const terrain* l);
point terrain_start_point(const terrain* l);
int terrain_height(const terrain* columns);
int terrain_width(const terrain* columns);

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void terrain_left(terrain* l);
void terrain_right(terrain* l);
void terrain_fall(terrain* columns);

#endif
