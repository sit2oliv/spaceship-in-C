#ifndef _POSITION_H_
#define _POSITION_H_

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

#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

typedef struct point
{
  int x;
  int y;
} point;

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

static inline point point_xy(int x, int y)
{
  return (point) { .x = x, .y = y, };
}

static inline point point_invalid(void)
{
  return (point) { .x = -1, .y = -1, };
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

static inline bool point_is_valid(point p)
{
  return p.x >= 0 && p.y >= 0;
}

static inline bool point_equals(point a, point b)
{
  return a.x == b.x && a.y == b.y;
}

static inline bool point_is_in_rectangle(
    point p, point up_left, point bottom_right)
{
  const int x = p.x;
  const int y = p.y;
  const point ul = up_left;
  const point br = bottom_right;
  return x >= ul.x && x < br.x && y >= ul.y && y < br.y;
}

#endif
