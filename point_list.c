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

#include "point_list.h"
#include <stdio.h>
#include <stdlib.h>

struct point_list
{
	point points;
	struct point_list *precedent;
	struct point_list *suivant;
};

////////////////////////////////////////////////////////////////////////////////
// local functions declarations
////////////////////////////////////////////////////////////////////////////////

static inline point_list *point_list_suivant(const point_list *l);

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

point_list* point_list_new(void)
{
  return NULL;
}

// recursive //
void point_list_destroy(point_list* const l)
{
  if(!l)
    return;

  point_list_destroy(l->suivant);
  free(l);
  return;
}

// non recursive //
/*
void point_list_destroy(point_list* const l)
{
  if(!l)
    return;
  
  point_list* parcours = l;

  while (parcours){
    parcours = l->suivant;
    free(l);
  }
}
*/

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

point point_list_get_point(const point_list* const l, size_t i)
{
  const point_list* tmp = l;
  size_t j;
  for(j = 0; j < i; j++)
    tmp = point_list_suivant(tmp);

  return tmp->points;
}

size_t point_list_get_size(const point_list* const l)
{
  const point_list* tmp = l;
  size_t cur = 0;

  while(tmp){
    tmp = point_list_suivant(tmp);
    cur++;
  }

  return cur;
}

bool point_list_contains(const point_list* const l, point p)
{
  bool resultat = false;
  const point_list* tmp = l;

  while(tmp && !resultat)
  {
    if (point_equals(tmp->points, p))
      resultat = true;
    
    tmp = point_list_suivant(tmp);
  }

  return resultat;
}

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

point_list* point_list_push_front(point_list* const l, point p)
{
  point_list* const nouveau_point = malloc(sizeof(*nouveau_point));
  //point_list* copier = l;
  nouveau_point->precedent = NULL;
  nouveau_point->points = p;

  if(l)
    nouveau_point->suivant = l;
  else
    nouveau_point->suivant = NULL;

  return nouveau_point;
}

point_list* point_list_push_back(point_list* const l, point p)
{
  point_list* nouveau_point = malloc(sizeof(*nouveau_point));
  nouveau_point->suivant = NULL;
  nouveau_point->points = p;

  if(!l){
    nouveau_point->precedent = NULL;
    return nouveau_point;
  }
  else{
    point_list* tmp = l;

    while(tmp->suivant)
      tmp = point_list_suivant(tmp);
    
    tmp->suivant = nouveau_point;
    nouveau_point->precedent = tmp;
    return l;
  }
}

point_list* point_list_pop_front(point_list* const l)
{
  if(!l->suivant){
    free(l);
    return NULL;
  }
  else{
    point_list* dierge = l->suivant;
    free(l);
    dierge->precedent = NULL;
    return dierge;
  }
}

point_list* point_list_pop_back(point_list* const l)
{
  point_list* tmp = l;

  while(tmp->suivant)
    tmp = point_list_suivant(tmp);

  tmp->precedent->suivant = NULL;
  free(tmp);
  return l;
}

void point_list_set_point(point_list* const l, size_t i, point p)
{
  point_list* tmp = l;
  size_t j;
  for(j = 0; j < i; j++)
    tmp = point_list_suivant(tmp);

  tmp->points = p;
  return;
}
int point_longeur (point_list* const l)
{
	point_list* tmp = l;
	int ll = 0;
	while(tmp)
	{
    ll++;  
    tmp = point_list_suivant(tmp);
  }
  return ll;
}

point_list* point_list_prune_out_of_bounds(
    point_list* const l, point up_left, point bottom_right)
{
  point_list* tmp = l;
  point_list* dierge = tmp;
  int ll=point_longeur(l);
  while(tmp)
  {
     //printf("test if we need to delete a point!");
    if(!point_is_in_rectangle(tmp->points, up_left, bottom_right))
    {
      if(!tmp->precedent)
        // free(parcours);
       { dierge = dierge->suivant;
        point_list_pop_front(tmp); }
      else{
        tmp->precedent->suivant = tmp->suivant;
        // free(parcours);
        point_list_pop_front(tmp);
      }
    } 
    else if(!point_is_valid(tmp->points))
    {	
		//printf("valid");
		tmp->points = point_invalid();

	}
    tmp = point_list_suivant(tmp);
  } 
  if(dierge)
    return dierge;
  else
    return NULL;
}

void point_list_shift_left(point_list* const l)
{
  point_list* tmp = l;

  while(tmp){
    tmp->points.x--;
    tmp = point_list_suivant(tmp);
  }
}

void point_list_shift_right(point_list* const l)
{
  point_list *tmp = l;

  while (tmp){
    tmp->points.x++;
    tmp = point_list_suivant(tmp);
  }
}

/*void point_list_shift(point_list *const l)
{
  point_list *tmp = l;

  while (tmp)
  {
    if (tmp->points.dir == BAS)
      ++tmp->points.y;
    if (tmp->points.dir == HAUT)
      --tmp->points.y;
    if (tmp->points.dir == AVANT)
      ++tmp->points.x;
      
    tmp = point_list_suivant(tmp);
  }
}

void point_list_print(point_list* const l)
{
  point_list* tmp = l;

  while(tmp){
    printf("[%d, %d] ", tmp->points.x, tmp->points.y);
    tmp = point_list_suivant(tmp);
  }
  printf("\n");
} */

////////////////////////////////////////////////////////////////////////////////
// local functions definitions
////////////////////////////////////////////////////////////////////////////////

point_list *point_list_suivant(const point_list *l)
{
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wcast-qual"
    return (point_list *)(l ? l->suivant : l);
  #pragma GCC diagnostic pop
}

// void point_list_shift_upper(point_list *const l)
// {
//   point_list *parcours = l;

//   while (parcours)
//   {
//     parcours->pt = point_xy(parcours->pt.x, parcours->pt.y - 1);
//     parcours = point_list_suivant(parcours);
//   }
// }

// void point_list_shift_lower(point_list *const l)
// {
//   point_list *parcours = l;

//   while (parcours)
//   {
//     parcours->pt = point_xy(parcours->pt.x, parcours->pt.y + 1);
//     parcours = point_list_suivant(parcours);
//   }
// }
