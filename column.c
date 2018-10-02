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
#include "column.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sysexits.h>

/* If you need other headers, include them here: */

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

/*
struct column
{
  cell* cells;
  int height;
};
*/

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

column* column_new(const int height, const int low, const int high)
{
  column* const c = malloc(sizeof *c);
  if (!c)
  {
    perror("malloc");
    exit(EX_OSERR);
  }
  cell* const cells = malloc(sizeof *cells * (size_t) height);
  if (!cells)
  {
    perror("malloc");
    exit(EX_OSERR);
  }

  for (int i = 0; i < height; ++i)
      cells[i] = i > high || i < low ? CELL_WALL : CELL_EMPTY;
  c->cells = cells;
  c->height = height;

  return c;
}

void column_destroy(column* const c)
{
  if (!c)
    return;

  if (c->cells)
    free(c->cells);
  free(c);
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

cell column_get_cell(const column* const c, const size_t i)
{
  return c && c->cells && i < (size_t) c->height ? c->cells[i] : CELL_EMPTY;
}

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

void column_set_cell(column* const c, const size_t i, const cell x)
{
  if (c)
    c->cells[i] = x;
}

void column_fall(column* const c)
{
	int high;
	int low;
	for (int i = 0; i < c->height; ++i)
	{ 
		if (c->cells[i] == CELL_EMPTY)
		{
			high = i;
			break;
		}
	}

	for (int j = c->height; j >0; j--)
	{
		if (c->cells[j] == CELL_EMPTY)
		{
			low = j;
			break;
		}
	}
	for (int t = low; t >high; t--)
	{
		if (c->cells[t-1] == CELL_WALL)
		c->cells[t] = c->cells[t - 1];
	}

	
  /* Write the code for this function. ! */
}
