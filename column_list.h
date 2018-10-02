/**
 * \file column_list.h
 * \brief Header pour les listes simplment chaînées de colonnes
 */ 

#ifndef _LISTE_H_
#define _LISTE_H_

#include <stddef.h>
#include "column.h"

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

/**
 * \struct column_list
 * \brief Une liste de colonnes est representée comme une liste simplement chainée
 * non circulaire. Plus precisement cette structure est composée 
 * d'un pointeur vers une colonne et d'un pointeur vers une autre liste de colonnes (la suivante).
 */

typedef struct column_list column_list;

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn column_list_new(void)
 * \brief Creer une liste de colonnes vide.
 *
 * \return une liste de colonnes vide
 */
column_list* column_list_new(void);

/**
 * \fn column_list_destroy(column_list* l)
 * \brief Detruit une liste de colonnes.
 *
 * \param l liste de colonnes
 * \return void, la liste a ete effacée par effet de bord
 */
void column_list_destroy(column_list* l);

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn column_list_get_column(const column_list* l, size_t i)
 * \brief Renvoie la ieme colonne de la liste.
 *
 * \param l liste de colonnes
 * \param i index de la colonne cherchée
 * \return la ieme colonne
 */
column* column_list_get_column(const column_list* l, size_t i);

/**
 * \fn column_list_get_size(const column_list* l)
 * \brief Renvoie la longueur de la liste de colonne.
 *
 * \param l liste de colonnes
 * \return la taille de la liste
 */
size_t column_list_get_size(const column_list* l);

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn column_list_push_front(column_list* l, column* c)
 * \brief Ajoute une colonne en tete de la liste.
 *
 * \param l liste de colonnes
 * \param c colonne a inserer en tete
 * \return la liste avec la nouvelle colonne en tete
 */
column_list* column_list_push_front(column_list* l, column* c);

/**
 * \fn column_list_push_back(column_list* l, column* c)
 * \brief Ajoute une colonne en queue de liste.
 *
 * \param l liste de colonnes
 * \param c colonne a inserer en queue
 * \return la liste avec la nouvelle colonne en queue
 */
column_list* column_list_push_back(column_list* l, column* c);

/**
 * \fn column_list_pop_front(column_list* l)
 * \brief Supprime la colonne en tete de la liste.
 *
 * \param l liste de colonnes
 * \return la liste avec sa tete qui a ete supprimée
 */
column_list* column_list_pop_front(column_list* l);

/**
 * \fn column_list_pop_back(column_list* l)
 * \brief Supprime la colonne en queue de liste.
 *
 * \param l liste de colonnes
 * \return la liste avec sa queue qui a ete supprimée
 */
column_list* column_list_pop_back(column_list* l);

#endif
