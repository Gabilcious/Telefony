#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#define BASESSIZE 110

struct PhoneForward *bases[BASESSIZE];
char *names[BASESSIZE];	 // Tablica nazw wszystkich baz przekierowań
int charNumber = 0;		 // 
bool isUngetted = false; // Służy sprawdzeniu, czy nie mamy znaka "cofniętego" do buffora 
char ungetted;			 // Znak, który został "cofnięty" do buffora
char *buff = "";		 // Buffor wejściowy
char *number = "";		 // Zmienna do przechowywania numeru przy operatorach jednoargumentowych
char *number1 = "";		 // Zmienna do przechowywania numeru przy operatorach dwuargumentowych
char *number2 = "";		 // Zmienna do przechowywania nuemru przy operatorach dwuargumentowych
char *id = "";			 // Zmienna do przechowywania nazwy bazy danych
char *name = "";		 // Zmienna do przechowywania nazwy operatora

// Własna funkcja zwalniająca pamięć
void free2(char *c) {
	if (strcmp(c,"")) {
		free(c);
		c = "";
	}
}

// Własna funkcja, zwracająca znak z wejścia
char getChar() {
	charNumber++;
	char res;
	if (isUngetted) {
		res = ungetted;
		isUngetted = false;
	}
	else {
		res = (char)getchar();
	}
	return res;
}

// Własna funkcja, umożliwiająca cofnięcie znaku na wejście, aby ponownie go wczytać
void ungetC(char c) {
	charNumber--;
	isUngetted = true;
	ungetted = c;
}

// Funkcja odwracająca tablicę charów
void reverse(char *s) {
     int i, j;
     char c;

     for (i = 0, j = strlen(s) - 1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

// Funkcja zmieniająca int na string - przekazywane jedynie dodatnie int
char* itoa(int num, char* str, int base)
{
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
    str[i] = '\0';
    reverse(str);
    return str;
}

// Funkcja obsługująca wyjjście z programu i zwolnienie pamięci
void terminate(int state, char *operator, int noLine) {
	free2(number);
	free2(number1);
	free2(number2);
	free2(id);
	free2(name);
	int i = 0;
	while (i < BASESSIZE) {
		if (bases[i] != NULL) {
			free2(names[i]);
			names[i] = "";
			phfwdDelete(bases[i]);
			bases[i] = NULL;
		}
		i++;
	}
	
	if (operator != NULL && !strcmp(operator, "EOF")) {
		fprintf(stderr, "ERROR EOF\n");
		exit(state);
	}

	if (state != 0) {
		fprintf(stderr, "ERROR ");

		if (operator != NULL) {
			fprintf(stderr, "%s ", operator);
		}
		char *res = (char *)malloc(20);
		res = itoa(noLine, res, 10);
		fprintf(stderr, "%s\n", res);
		free2(res);
	}
	exit(state);
}

// Funkcja zwracająca połączenie c2 z c1
char *glue(char *c1, char c2) {
	int c1len = strlen(c1);
	char *res = (char *)malloc(c1len + 2);
	if (res == NULL) {
		terminate(1, "Nie udało się zamallocować pamięci", 0);
	}
	strcpy(res,c1);
	free2(c1);
	res[c1len] = c2;
	res[c1len + 1] = '\0';
	return res;
}

// Funkcja sprawdzająca, czy char jest białym znakiem
bool isWhiteSpace(char c) {
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}

// Funkcja Sprawdzająca, czy następny wczytywany znak to EOF
bool isEOF() {
	char c = getChar();
	ungetC(c);
	return (c == EOF);
}

// Funkcja ignorująca komentarz
void waitForComment() {
	char c = getChar();
	if (c == '$') {
		if (isEOF()) {
			terminate(1, NULL, charNumber);
		}
		c = getChar();
		if (c == '$') {
			while (true) {
				if (isEOF()) {
					terminate(1, "EOF", -1);
				}
				c = getChar();
				if (c == '$') {
					c = getChar();
					if (c == '$') {
						break;
					}
				}
			}
		}
		else {
			terminate(1, NULL, charNumber - 1);
		}
	}
	else {
		ungetC(c);
	}
}

// Funkcja ignorująca białe znaki
void waitForWhiteSpaces() {
	char c = getChar();
	while (isWhiteSpace(c)) {
		c = getChar();
	}
	ungetC(c);
}

// Funkcja ignorująca komentarze oraz białe znaki
void pass() {
	waitForWhiteSpaces();
	char c = getChar();
	while (c == '$') {
		ungetC(c);
		waitForComment();
		waitForWhiteSpaces();
		c = getChar();
	}
	ungetC(c);
}

// Funkcja zwracająca ze standardowego wejścia ciąg znaków, który jest numerem
char *getNumber() {
	if (isEOF()) {
		terminate(1, "EOF", -1);
	}
	
	char c;
	buff = "";
 	while ((c = getChar())) {
		if (isDigit(c)) {
			buff = glue(buff, c);
		}
		else {
			ungetC(c);
			break;
		}
	}
	return buff;
}

// Funkcja zwracająca ze standardowego wejścia ciąg znaków, który jest identyfikatorem
char *getID() {
	if (isEOF()) {
		terminate(1, "EOF", -1);
	}

	char c = getChar();
	if (!isalpha(c)) {
		ungetC(c);
		return "";
	}

	buff = "";
 	while (true) {
		if (isalpha(c) || isDigit(c)) {
			buff = glue(buff, c);
		}
		else {
			ungetC(c);
			break;
		}
		c = getChar();
	}
	return buff;
}


int main() {
	char c;
	int i = 0;
	while (i < BASESSIZE) {
		bases[i] = NULL;
		names[i] = "";
		i++;	
	}
	int actual = -1;
	
	// Jeden obrót pętli to jedna instrukcja do wykonania
	while (true) {
		pass();
		c = getChar();
		int firstCommandNum = charNumber;

		// Instrukcja zaczyna się od numeru
		if (isDigit(c)) {
			ungetC(c);
			free2(number1);
			number1 = getNumber();
			pass();
			c = getChar();
			
			// Dodawanie przekierowania
			if (c == '>') {
				firstCommandNum = charNumber;
				pass();
				free2(number2);
				number2 = getNumber();
				
				if (!strcmp(number2, "")) {
					terminate(1, NULL, charNumber + 1);
				}

				if (actual == -1 || !phfwdAdd(bases[actual], number1, number2)) {
					terminate(1, ">", firstCommandNum);
				}
			}
			else if (c == '?') {
				// Sprawdzanie przekierowań z podanego numeru
				firstCommandNum = charNumber;
				if (actual == -1) {
					terminate(1, "?", firstCommandNum);
				}

				struct PhoneNumbers const *res = phfwdGet(bases[actual], number1);
				
				if (phnumGet(res,0) == NULL) {
					phnumDelete(res);
					terminate(1, "?", firstCommandNum);
				}
				
				printf("%s\n", phnumGet(res, 0));
				phnumDelete(res);
			}
			else if (c == EOF) {
				terminate(1, "EOF", -1);
			}
			else {
				terminate(1, NULL, charNumber);
			}
		}


		// Instrukcja zaczyna się od NEW lub DEL
		else if (isalpha(c)) {
			if (c != 'N' && c != 'D') {
				terminate(1, NULL, charNumber);
			}
			ungetC(c);
			free2(id);
			id = getID();
			
			if (strcmp(id,"DEL") && strcmp(id,"NEW")) {
				terminate(1, NULL, firstCommandNum);
			}

			pass();
			c = getChar();

			// Instrukcja zaczyna się od NEW
			if (!strcmp(id, "NEW")) {
				if (c == EOF) {
					terminate(1, "EOF", -1);
				}
				if (!isalpha(c)) {
					terminate(1, NULL, charNumber);
				}
				
				ungetC(c);
				free2(name);
				name = getID();
				
				// Przypadek, gdy identyfikator próbuje przyjąć nazwę zastrzeżoną
				if (!strcmp(name,"DEL") || !strcmp(name,"NEW")) {
					terminate(1, NULL, charNumber - 2);
				}
				
				// Sprawdzenie, czy już istnieje dana baza
				// Jeśli tak - ustawienie jej jako aktualnej
				bool exist = false;
				int i = 0;
				while (i < BASESSIZE) {
					if (!strcmp(names[i], name)) {
						actual = i;
						exist = true;
					}
					i++;
				}
				// Jeśli baza nie istnieje - stworzenie jej
				if (!exist) {
					i = 0;
					while (i < BASESSIZE) {
						if (!strcmp(names[i], "")) {
							actual = i;
							names[i] = malloc(strlen(name)+1);
							if (names[i] == NULL) {
								terminate(1, "Nie udało się zamallocować pamięci", 0);
							}
							strcpy(names[i],name);
							bases[i] = phfwdNew();
							break;
						}
						i++;
					}
				}
			}
			// Instrukcja zaczyna się od DEL
			else if (!strcmp(id, "DEL")) {
				// Usuwanie przekierowania
				if (isDigit(c)) {
					ungetC(c);
					free2(number);
					number = getNumber();

					if (actual == -1) {
						terminate(1, "DEL", firstCommandNum);
					}

					phfwdRemove(bases[actual], number);
					number = "";
				}
				// Usuwanie bazy danych
				else if (isalpha(c)) {
					ungetC(c);
					free2(name);
					name = getID();
					
					if (!strcmp(name,"NEW") || !strcmp(name, "DEL")) {
						terminate(1, NULL, charNumber - 2);
					}
					
					int i = 0;
					bool deleted = false;
					while (i < BASESSIZE) {
						if (!strcmp(names[i], name)) {
							phfwdDelete(bases[i]);
							bases[i] = NULL;
							/** free2(names[i]); */
							if(strcmp(names[i],"")) {
								free(names[i]);
								names[i]="";
							}
							if (i == actual) {
								actual = -1;
							}
							deleted = true;
							break;
						}
						i++;
					}
					// Jeśli baza nie została usunięta, to nie istnieje, więc zgłaszamy błąd
					if (!deleted) {
						terminate(1, "DEL", firstCommandNum);
					}
				}
				else if (c == EOF) {
					terminate(1, "EOF", -1);
				}
				else {
					terminate(1, NULL, charNumber);
				}
			}
		}
		// Instrukcja to sprawdzanie przekierowań na podany numer
		else if (c == '?') {
			pass();
			free2(number);
			number = getNumber();
			
			if (!strcmp(number,"")) {
				terminate(1, NULL, charNumber + 1);
			}

			if (actual == -1) {
				terminate(1, "?", firstCommandNum);
			}

			struct PhoneNumbers const *res = phfwdReverse(bases[actual], number);
			int i = 0;

			if (phnumGet(res, 0) == NULL) {
				phnumDelete(res);
				terminate(1, "?", firstCommandNum);
			}

			while (phnumGet(res, i) != NULL) {
				printf("%s\n", phnumGet(res,i));
				i++;
			}

			phnumDelete(res);
		}
		else if (c == '@') {
			pass();
			free2(number);
			number = getNumber();
			
			if (!strcmp(number,"")) {
				terminate(1, NULL, charNumber + 1);
			}

			if (actual == -1) {
				terminate(1, "@", firstCommandNum);
			}
			size_t len = 0;
			if (strlen(number) > 12) {
				len = strlen(number) - 12;
			}
			size_t res = phfwdNonTrivialCount(bases[actual], number, len); //

			printf("%lu\n", res);
		}
		else if (c == EOF) {
			terminate(0, NULL, -1);
		}
		else {
			terminate(1, NULL, charNumber);
		}
	}
}
