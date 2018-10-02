#ifndef _COLONNE_H_
#define _COLONNE_H_

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

#include <stddef.h>
#include "cell.h"

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef struct column column;

struct column
{
	cell* cells;
	int height;
};

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

column* column_new(int height, int low, int high);
void column_destroy(column* c);

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

cell column_get_cell(const column* c, size_t i);

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void column_set_cell(column* c, size_t i, cell x);
void column_fall(column* c);

#endif
