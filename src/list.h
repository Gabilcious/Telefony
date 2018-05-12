/** @file
 * Interfejs listy jednokierunkowej
 *
 * @author Karolina Gabara
 * @copyright Uniwersytet Warszawski
 * @date 09.05.2018
 */

#ifndef __list_h__
#define __list_h__
#include <stdbool.h>

/**
 * Struktura Listy
 */
typedef struct List {
    char *id;			///< wartość węzła
    struct List *next;  ///< wskaźnik na następny element
} List;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę, której następnikiem jest @p next, a wartość węzła wynosi @p id
 * @param[in] id   – wartość węzła w nowej liście
 * @param[in] next – następnik
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
List * newList(const char *id, List *next);

/** @brief Dodaje następnik do listy.
 * Dodaje następnik do listy.
 * @param[in] head – głowa listy
 * @param[in] id – wartość następnika
 */
void add(List *head, const char *id);

/** @brief Usuwa następnik z listy.
 * Usuwa następnik z listy. Następnik następnika staje się nowym następnikiem.
 * @param[in] head – głowa listy
 */
void removeNext(List *head);

/** @brief Usuwa z listy elemenent o konkretnej wartości węzła.
 * Usuwa z listy elemenent o konkretnej wartości węzła.
 * @param[in] head – głowa listy
 * @param[in] id – wartość elementu do usunięcia
 */
void removeConcrete(List *head, char *id);

/** @brief Czyści listę.
 * Czyści listę, lista przybiera wartość NULL
 * @param[in] head – głowa listy
 */
void clear(List *head);

/** @brief Sprawdza czy jest pusta
 * Sprawdza czy jest pusta
 * @param[in] head – głowa listy
 */
inline bool isEmpty(List *head) {
    return (head->next == NULL);
}

#endif
