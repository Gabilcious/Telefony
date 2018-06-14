/** @file
 * Interfejs klasy ForwardTree
 *
 * @author Karolina Gabara
 * @copyright Uniwersytet Warszawski
 * @date 09.05.2018
 */

#ifndef __FORWARD_TREE_H__
#define __FORWARD_TREE_H__
#include "phone_forward_config.h"

/**
 * Struktura przechowująca przekierowanie telefonów.
 */
typedef struct ForwardTree {
    char *from;						///< napis oryginalny, który zostaje przekierowany
    char *to;						///< napis wtórny, na który zostaje przekierowany napisy oryginalny
    int lastDigFrom;				///< ostatnia cyfra napisu oryginalnego
    struct ForwardTree **next;		///< tablica wskaźników na przekierowania, których napisy oryginalne składają się z @p from oraz jednego znaku
    struct ForwardTree *prev;		///< wskaźnik na przekierowanie, którego napis oryginalny składa się z @p from bez ostatniego znaku
} ForwardTree;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę.
 * @param[in] prev – wskaźnik na poprzednik nowej struktury.
 * @param[in] lastDigFrom – ostatnia cyfra napisu oryginalnego nowej struktury
 * @param[in] from – wskaźnik na napis oryginalny nowej struktury
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
ForwardTree * forNewNode(ForwardTree *prev, int lastDigFrom, const char *from);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p head. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void forRemoveNode(ForwardTree *head);

/** @brief Usuwa puste węzły.
 * Usuwa rekurencyjnie wszystkie puste węzły od @p head.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL lub @p head nie jest pustym węzłem.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void forClearFromNode(ForwardTree *head);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p head oraz rekurencyjnie wszystkie dzieci.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void forRemoveAllNodes(ForwardTree *head);

#endif
