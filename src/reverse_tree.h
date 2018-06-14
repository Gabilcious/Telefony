/** @file
 * Interfejs klasy ReverseTree
 *
 * @author Karolina Gabara
 * @copyright Uniwersytet Warszawski
 * @date 09.05.2018
 */

#ifndef __REVERSE_TREE_H__
#define __REVERSE_TREE_H__
#include "list.h"
#include "phone_forward_config.h"


/**
 * Struktura przechowująca odwrócone przekierowania telefonów.
 */
typedef struct ReverseTree {
    char *to;						///< napis wtórny, na który zostają przekierowane napisy oryginalne
    List *from;						///< lista napisów oryginalnych, które są przekierowane na napis wtórny
    int lastDigTo;					///< ostatnia cyfra napisu wtórnego
    int forwardCount;				///< liczba napisów oryginalnych, które są przekierowywane na napis wtórny	
    struct ReverseTree **next;		///< tablica wskaźników na przekierowania, których napisy wtórne składają się z @p from oraz jednego znaku
    struct ReverseTree *prev;		///< wskaźnik na przekierowanie, którego napis wtórny składa się z @p from bez ostatniego znaku	
} ReverseTree;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę.
 * @param[in] prev – wskaźnik na poprzednik nowej struktury.
 * @param[in] lastDigTo – ostatnia cyfra napisu wtórnego nowej struktury
 * @param[in] to – wskaźnik na napis wtórny nowej struktury
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
ReverseTree * revNewNode(ReverseTree *prev, int lastDigTo, const char *to);

/** @brief Usuwa przekierowanie z @p from
 * Usuwa @p from z listy napisów oryginalnych @p head.
 * @param[in] head – wskaźnik na strukturę, w której należy dokonać zmiany.
 * @param[in] from – napis, który należy usunąć.
 */
void revDeleteFromNode(ReverseTree *head, char *from);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p head. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void revRemoveNode(ReverseTree *head);

/** @brief Usuwa konkretny węzeł.
 * Usuwa przekierowanie z @p from z węzła o napisie wtórnym równym @p to.
 * @param[in] head – wskaźnik na strukturę.
 * @param[in] to – napis wtórny.
 * @param[in] from – napis oryginalny.
 */
void revRemoveConcreteNode(ReverseTree *head, char *to, char *from);

/** @brief Usuwa puste węzły.
 * Usuwa rekurencyjnie wszystkie puste węzły od @p head.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL lub @p head nie jest pustym węzłem.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void revClearFromNode(ReverseTree *head);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p head oraz rekurencyjnie wszystkie dzieci.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] head – wskaźnik na usuwaną strukturę.
 */
void revRemoveAllNodes(ReverseTree *head);

#endif
