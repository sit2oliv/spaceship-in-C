/**
 * \file point_list.h
 * \brief Header pour les listes doublement chaînées de points
 */

#ifndef _POINT_LIST_H_
#define _POINT_LIST_H_

#include <stddef.h>
#include <stdbool.h>

#include "point.h"

////////////////////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////////////////////

/**
 * \struct point_list
 * \brief Une liste de points est representée comme une liste doublement chainée
 * non circulaire. Plus precisement cette structure est composée 
 * d'un point et de deux pointeurs vers une autre liste de points (la suivante et la precedente).
 */

typedef struct point_list point_list;

////////////////////////////////////////////////////////////////////////////////
// init./destroy etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn point_list_new(void)
 * \brief Creer une liste de points vide.
 *
 * \return une liste de points vide
 */
point_list* point_list_new(void);

/**
 * \fn point_list_destroy(point_list* l)
 * \brief Detruit une liste de points.
 *
 * \param l liste de points
 * \return void, la liste a ete effacée par effet de bord
 */
void point_list_destroy(point_list* l);

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn point_list_get_size(const point_list* l)
 * \brief Renvoie la longueur de la liste de points.
 *
 * \param l liste de points
 * \return la taille de la liste
 */
size_t point_list_get_size(const point_list* l);

/**
 * \fn point_list_get_point(const point_list* l, size_t i)
 * \brief Renvoie le ieme point de la liste.
 *
 * \param l liste de points
 * \param i index du point cherchée
 * \return le ieme point
 */
point point_list_get_point(const point_list* l, size_t i);

/**
 * \fn point_list_contains(const point_list* l, point p)
 * \brief Teste si un point appartient a une liste de points.
 *
 * \param l liste de points
 * \param p point
 * \return un booleen indiquant si p appartient a l
 */
bool point_list_contains(const point_list* l, point p);

////////////////////////////////////////////////////////////////////////////////
// setters / modifiers
////////////////////////////////////////////////////////////////////////////////

/**
 * \fn point_list_set_point(point_list* l, size_t i, point p)
 * \brief Fixe le point p a la ieme place de la liste l.
 *
 * \param l liste de points
 * \param i index de l'element
 * \param p point
 * \pre point_list_get_size(l) > i
 * \return void mais le point se trouvant a la ieme place de l est devenu p
 */
void point_list_set_point(point_list* l, size_t i, point p);

/**
 * \fn point_list_push_front(point_list* l, point p)
 * \brief Ajoute un point en tete de la liste.
 *
 * \param l liste de points
 * \param p point a inserer en tete
 * \return la liste avec le nouveau point en tete
 */
point_list* point_list_push_front(point_list* l, point p);

/**
 * \fn point_list_push_back(point_list* l, point p)
 * \brief Ajoute un point en queue de liste.
 *
 * \param l liste de points
 * \param p point a inserer en queue
 * \return la liste avec le nouveau point en queue
 */
point_list* point_list_push_back(point_list* l, point p);

/**
 * \fn point_list_pop_front(point_list* l)
 * \brief Supprime le point en tete de la liste.
 *
 * \param l liste de points
 * \pre l non vide
 * \return la liste avec sa tete qui a ete supprimée
 */
point_list* point_list_pop_front(point_list* l);

/**
 * \fn point_list_pop_back(point_list* l)
 * \brief Supprime le point en queue de la liste.
 *
 * \param l liste de points
 * \pre l non vide
 * \return la liste avec sa queue qui a ete supprimée
 */
point_list* point_list_pop_back(point_list* l);

/**
 * \fn point_list_shift_left(point_list* l)
 * \brief Fait reculer d'une unité tout les points de la liste.
 *
 * \param l liste de points
 * \return void mais les points on tous été modifiés
 */
void point_list_shift_left(point_list* l);

/**
 * \fn point_list_shift_right(point_list* l)
 * \brief Fait avancer d'une unité tout les points de la liste.
 *
 * \param l liste de points
 * \return void mais les points on tous été modifiés
 */
void point_list_shift_right(point_list* l);

/**
 * \fn point_list_shift(point_list* l)
 * \brief Fait avancer d'une unité tout les points selon leurs direction.
 *
 * \param l liste de points
 * \return void mais les points on tous été modifiés
 */
void point_list_shift(point_list *const l);

/**
 * \fn point_list_prune_out_of_bounds(
    point_list *l, point up_left, point bottom_right)
 * \brief Supprime les points de la liste qui ne se trouve pas dans le rectangle
 * definie par son coin haut gauche et son coin bas droit.
 *
 * \param l liste de points
 * \return la liste dont certains points ont ete supprimés
 */
point_list* point_list_prune_out_of_bounds(
    point_list *l, point up_left, point bottom_right);

/**
 * \fn point_list_print(point_list* l)
 * \brief Affiche la liste de points sur le terminal.
 *
 * \param l liste de points
 * \return void mais la liste a ete affichée
 */
void point_list_print(point_list* l);

#endif
