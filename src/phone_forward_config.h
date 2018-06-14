/** @file
 * Konfiguracja i podstawowe funkcje numeru telefonu
 *
 * @author Karolina Gabara
 * @copyright Uniwersytet Warszawski
 * @date 02.06.2018
 */

#ifndef __PHONE_FORWARD_CONFIG_H__
#define __PHONE_FORWARD_CONFIG_H__

#include <stdbool.h>
#include <ctype.h>
/**
 * Stała, oznaczająca pustą tablicę charów
 */
static const char EMPTY[] = "empty";
/**
 * Stała, określająca liczbę znaków dozwolonych w danej specyfikacji
 * numerów telefonu.
 */
static const int SIZE = 12;

/** @brief Sprawdza, czy @p c jest znakiem
 * Funkcja zwraca true, jeśli @p c jest dozwolonym znakiem
 * w numerze i false, jeżeli nie jest
 * @param[in] c - znak, który należy sprawdzić
 * @return Czy @p c jest dozwolonym znakiem.
 */
static inline bool isDigit(char c) {
	return (isdigit(c) || c== ':' || c == ';');
}

/** @brief Zwraca int odpowiadający znakowi @p c
 * Funkcja zwraca int odpowiadający znakowi @p c, w danej specyfikacji
 * numerów telefonu.
 * @param[in] c - Znak, którego odpowiednik należy zwrócić
 * @return Int odpowiadający znakowi
 */
static inline int getDigit(char c) {
	if (isdigit(c)) {
		return ((int)c - 48);
	}
	else if (c == ':') {
		return 10;
	}
	else {
		return 11;
	}
}

#endif
