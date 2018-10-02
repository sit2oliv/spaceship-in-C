/**
 * \file column_list.c
 * \brief Listes simplement chainées de colonnes
 */ 

#include "column_list.h"

#include <stdlib.h>

struct column_list
{
	column *c;
	struct column_list *suivant;
};

////////////////////////////////////////////////////////////////////////////////
// local functions declarations
////////////////////////////////////////////////////////////////////////////////

static inline column_list *column_list_suivant(const column_list *l);

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

column_list *column_list_new(void)
{
  return NULL;
}

void column_list_destroy(column_list* const l)
{
  if(!l)
    return;
  column_list *tmp = l;
  while(tmp) {
    tmp = l->suivant;
    if(l->c->cells)
      free(l->c->cells);
    free(l->c);
    free(l);
  } 
  return;
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

column* column_list_get_column(const column_list* const l, const size_t i)
{
  size_t j = 0;
  const column_list* tmp = l;
  while(j != i) {
    tmp = column_list_suivant(tmp);
    j++;
  }
  return tmp->c;
}

size_t column_list_get_size(const column_list* const l)
{
  size_t j = 0;
  const column_list* tmp = l;
  while(tmp) {
    tmp = column_list_suivant(tmp);
    j++;
  }
  return j;
}

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

column_list* column_list_push_front(column_list* const l, column* const c)
{
  column_list* const nouvelle_colonne = malloc(sizeof(*nouvelle_colonne));
  nouvelle_colonne->c = c;
  nouvelle_colonne->suivant = l;
  return nouvelle_colonne;
}

column_list* column_list_push_back(column_list* const l, column* const c)
{
  column_list* const nouvelle_colonne = malloc(sizeof(*nouvelle_colonne));
  column_list *tmp = l;

  while (tmp->suivant)
    tmp = column_list_suivant(tmp);

  nouvelle_colonne->c = c;
  nouvelle_colonne->suivant = NULL;
  tmp->suivant = nouvelle_colonne;

  return l;
}

column_list *column_list_pop_front(column_list *const l)
{
  if(!l->suivant){
    free(l);
    return NULL;
  }
  else{
    column_list* cur = l->suivant;
    free(l);
    return cur;
  }
}

column_list* column_list_pop_back(column_list* const l)
{
  column_list *tmp = l;

  while (tmp->suivant)
    tmp = column_list_suivant(tmp);

  free(tmp);

  return l;
}

////////////////////////////////////////////////////////////////////////////////
// local functions definitions
////////////////////////////////////////////////////////////////////////////////

column_list *column_list_suivant(const column_list *l)
{
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wcast-qual"
    return (column_list *)(l ? l->suivant : l);
  #pragma GCC diagnostic pop
}
