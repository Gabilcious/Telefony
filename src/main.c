#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#define BASESSIZE 110

struct PhoneForward *bases[BASESSIZE];
char *names[BASESSIZE];
int charNumber = 0;
bool isUngetted = false;
char ungetted;

inline char getChar() {
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

inline void ungetC(char c) {
	charNumber--;
	isUngetted = true;
	ungetted = c;
}

void reverse(char *s) {
     int i, j;
     char c;
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

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

// Dokleja c2 na koniec c1
void glue(char *c1, char c2) {
	int c1len = strlen(c1);
	char *res = (char *)malloc(c1len + 2);
	strcpy(res,c1);
	res[c1len] = c2;
	res[c1len + 1] = '\0';
	free(c1);
	c1 = res;
}

void terminate(int state, char *operator, int noLine) {
	int i = 0;
	while (i < BASESSIZE) {
		free(names[i]);
		names[i] = NULL;
		phfwdDelete(bases[i]);
		bases[i] = NULL;
		i++;
	}
	
	if (!strcmp(operator, "EOF")) {
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
		free(res);
	}
	exit(state);
}

inline bool isWhiteSpace(char c) {
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}

void waitForComment() {
	char c = getChar();
	if (c == '$') {
		c = getChar();
		if (c == '$') {
			while (true) {
				c = getChar();
				if (c == '$') {
					c = getChar();
					if (c == '$') {
						break;
					}
				}
			}
		}
	}
	else {
		ungetC(c);
	}
}

void waitForWhiteSpaces() {
	char c = getChar();
	while (isWhiteSpace(c)) {
		c = getChar();
	}
	ungetC(c);
}

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

bool isEOF() {
	char c = getChar();
	ungetC(c);
	return (c == EOF);
}

char *getNumber() {
	if (isEOF()) {
		terminate(1, "EOF", -1);
	}
	
	char c;
	char *res = "";
 	while ((c = getChar())) {
		if (isdigit(c)) {
			glue(res, c);
		}
		else if (c == '$') {
			ungetC(c);
			waitForComment();
		}
		else {
			ungetC(c);
			break;
		}
	}
	return res;
}

char *getID() {
	if (isEOF()) {
		terminate(1, "EOF", -1);
	}

	char c = getChar();
	if (!isalpha(c)) {
		ungetC(c);
		return "";
	}

	char *res = "";
 	while ((c = getChar())) {
		if (isalpha(c) || isdigit(c)) {
			glue(res, c);
		}
		else if (c == '$') {
			ungetC(c);
			waitForComment();
		}
		else {
			ungetC(c);
			break;
		}
	}
	return res;
}

int main() {
	char c;
	int i = 0;
	while (i < BASESSIZE) {
		bases[i] = NULL;
		names[i] = NULL;
		//                  <---- TUTAJ
		//i++;
	}
	int actual = -1;
	while (true) {
		printf("bb\n");
		pass();
		printf("aa\n");
		
		c = getChar();
		int firstCommandNum = charNumber;
		if (isdigit(c)) {
			printf("isdigit\n");
			ungetC(c);
			char *number1 = getNumber();
			pass();
			c = getChar();
			if (c == '>') {
				pass();
				char *number2 = getNumber();
				
				if (!strcmp(number2, "")) {
					terminate(1, NULL, charNumber + 1);
				}

				if (actual == -1 || !phfwdAdd(bases[actual], number1, number2)) {
					terminate(1, ">", firstCommandNum);
				}	
			}
			else if (c == '?') {
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
		}
		else if (isalpha(c)) {
			printf("isalphaa\n");
			ungetC(c);
			char *id = getID();
			pass();
			if (strcmp(id, "NEW")) {
				printf("NEW\n");
				c = getChar();
				if (!isalpha(c)) {
					terminate(1, NULL, charNumber);
				}
				
				ungetC(c);
				char *name = getID();
				bool exist = false;
				int i = 0;
				while (i < BASESSIZE) {
					if (strcmp(names[i], name)) {
						actual = i;
						exist = true;
					}
					i++;
				}
				if (!exist) {
					i = 0;
					while (i < BASESSIZE) {
						if (names[i] == NULL) {
							actual = i;
							names[i] = name;
							bases[i] = phfwdNew();
						}
						i++;
					}
				}
			}
			else if (!strcmp(id, "DEL")) {
				printf("DEL\n");
				c = getChar();
				if (isdigit(c)) {
					ungetC(c);
					char *number = getNumber();

					if (actual == -1) {
						terminate(1, "DEL", firstCommandNum);
					}

					phfwdRemove(bases[actual], number);
				}
				else if (isalpha(c)) {
					ungetC(c);
					char *name = getID();
					
					if (!strcmp(name,"NEW") || !strcmp(name, "DEL")) {
						terminate(1, NULL, charNumber - 2);
					}
					
					int i = 0;
					while (i < BASESSIZE) {
						if (strcmp(names[i], name)) {
							phfwdDelete(bases[i]);
							bases[i] = NULL;
							names[i] = NULL;
							break;
						}
						i++;
					}
				}
				else {
					terminate(1, NULL, charNumber);
				}
			}
			else {
				terminate(1, NULL, firstCommandNum);
			}
			
		}
		else if (c == '?') {
			pass();
			char *number = getNumber();
			
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
				printf("%s", phnumGet(res,i));
				i++;
			}

			phnumDelete(res);
		}
		else if (c == EOF) {
			terminate(0, NULL, -1);
		}
		else {
			terminate(1, NULL, charNumber);
		}
	}
}
