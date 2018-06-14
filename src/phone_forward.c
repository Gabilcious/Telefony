#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "phone_forward.h"

struct PhoneForward *phfwdNew(void) {
    struct PhoneForward *element;
    element = (struct PhoneForward *)malloc(sizeof(PhoneForward));
	if (element == NULL) {
		return NULL;
	}
    element->forward = forNewNode(NULL, -1, EMPTY);
    element->reverse = revNewNode(NULL, -1, EMPTY);
    if (element->forward == NULL || element->reverse == NULL) {
        return NULL;
	}
    return element;
}

void phfwdDelete(struct PhoneForward *pf) {
    if (pf == NULL) {
		return;
	}
    forRemoveAllNodes(pf->forward);
    revRemoveAllNodes(pf->reverse);
    free(pf);
}

// Sprawdza, czy napis składa się z samych cyfr
static inline bool isNumber(char const *ch) {
    int i = 0;
    while (ch[i] != 0) {
		if (!isDigit(ch[i++])) {
			return false;
		}
	}
    return (strlen(ch) > 0);
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {
    if (!isNumber(num1) || !isNumber(num2) || !strcmp(num1, num2)) {
		return false;
	}

	// Znajduje wierzchołek w forwardTree odpowiadający num1
	// Jeśli nie istnieje ścieżka do niego - tworzy ją
	ForwardTree *forward = pf->forward;
    int i = 0;
    while (num1[i] != 0) {
        int digit = getDigit(num1[i]);
        if (forward->next[digit] == NULL) {
            char *from = (char *)malloc(i + 2);
            strncpy(from, num1, i + 1);
            from[i + 1] = 0;
            forward->next[digit] = forNewNode(forward, digit, from);
			free(from);

            if (forward->next[digit] == NULL) {
                forClearFromNode(forward);
                return false;
            }
        }
        forward = forward->next[digit];
        i++;
    }
	// Jeśli wcześniej istniało przekierowanie z num1 - usuwa je
    if (strcmp(forward->to, EMPTY)) {
        revRemoveConcreteNode(pf->reverse, forward->to, forward->from);
    }

	// Znajduje wierzchołek w reverseTree odpowiadający num2
	// Jeśli nie istnieje ścieżka do niego - tworzy ją
    ReverseTree *reverse = pf->reverse;
	i = 0;
    while (num2[i] != 0) {
		int digit = getDigit(num2[i]);
        if (reverse->next[digit] == NULL) {
			char *to = (char *)malloc(i + 2);
			strncpy(to, num2, i + 1);
			to[i + 1] = 0;
			reverse->next[digit] = revNewNode(reverse, digit, to);
			free(to);
			
			if (reverse->next[digit] == NULL) {
				forClearFromNode(forward);
                revClearFromNode(reverse);
                return false;
            }
        }
        reverse = reverse->next[digit];
        i++;
    }
	
	// Tworzy przekierowanie
    free(forward->to);
    forward->to = (char *)malloc(strlen(num2) + 1);
    strcpy(forward->to, num2);
    add(reverse->from, num1); 
    reverse->forwardCount++;
	return true;
}

// Rekurencyjnie usuwa przekierowania w całym poddrzewie
static void recPhfwdRemove(ForwardTree *forward, ReverseTree *reverse) {
    int i;
    for (i = 0; i < SIZE; i++) {
        if (forward->next[i] != NULL) {
            recPhfwdRemove(forward->next[i], reverse);
		}
	}
	// Usuwa wierzchołek w reverseTree, odpowiadający wierzchołkowi forward
    if (strcmp(forward->to, EMPTY)) {
        revRemoveConcreteNode(reverse, forward->to, forward->from);
    }
    forRemoveNode(forward);
}

void phfwdRemove(struct PhoneForward *pf, char const *num) {
    if (num == NULL || strlen(num) == 0) {
		return;
	}
   
	// Znajduje wierzchołek w forward, odpowiadający num
    ForwardTree *forward = pf->forward;
    int i = 0;
    while (num[i] != 0) {
        int digit = getDigit(num[i]);
        if (forward->next[digit] == NULL) {
			return;
		}
        forward = forward->next[digit];
        i++;
    }
    ForwardTree *prev = forward->prev;
    recPhfwdRemove(forward, pf->reverse);
    forClearFromNode(prev);
}

// Zwraca napis będący sklejeniem first z second
// Pomijamy pierwsze n znaków z napisu second
static char *connect(char *first, char const *second, int n){
    char *res;
    int firstLen = (int)strlen(first);
    int size = firstLen + (int)strlen(second) - n;
    res = (char *)malloc(size + 1);
    strcpy(res, first);
    strcpy(res + firstLen, second + n);
    return res;
}

// Zwraca wskaźnik na nową strukturę PhoneNumbers, niezawierającą żadnych numerów
static struct PhoneNumbers *phnumNew(int size) {
    struct PhoneNumbers *element;
    element = (struct PhoneNumbers *)malloc(sizeof(PhoneNumbers));
	if (element == NULL) {
		return NULL;
	}
    element->list = (char **)malloc(size * sizeof(char *));
	if (element->list == NULL) {
        free(element);
        return NULL;
    }
    element->size = size;
    int i;
    for (i = 0; i < size; i++) {
		element->list[i] = NULL;
	}
    return element;
}


struct PhoneNumbers const *phfwdGet(struct PhoneForward *pf, char const *num) {
    if (!isNumber(num)) {
		return phnumNew(0);
	}
    struct PhoneNumbers *res = phnumNew(1);
    if (res == NULL) {
        return NULL;
	}

	// Znajduje wierzchołek w forward, odpowiadający num
    ForwardTree *forward = pf->forward;
    int i = 0;
    while (num[i] != 0) {
        int digit = getDigit(num[i]);
        if (forward->next[digit] == NULL) {
			break;
		}
        forward = forward->next[digit];
        i++;
    }

	// Cofa się w drzewie do korzenia
	// W momencie znalezienia przekierowania - zwraca je
    while (forward->prev != NULL) {
        if (strcmp(forward->to, EMPTY)) {
            res->list[0] = connect(forward->to, num, i);

			if (res->list[0] == NULL) {
                phnumDelete(res);
                return NULL;
            }
            return res;
        }
        forward = forward->prev;
        i--;
    }

	// Jeśli nie znaleziono żadnego przekierowania - zwraca oryginalny napis
    res->list[0] = (char *)malloc(strlen(num) + 1);
	if (res->list[0] == NULL) {
		phnumDelete(res);
        return NULL;
    }
    strcpy(res->list[0], num);
    return res;
}

// Zwraca, czy napis L jest mniejszy leksykograficznie od napisu R
static bool isSmaller(char *L, char *R) {
    if (L == NULL) {
		return false;
	}
    if (R == NULL) {
		return true;
	}

    int sizeL = strlen(L);
    int sizeR = strlen(R);
    int i;
    for (i = 0; i < sizeL && i < sizeR; i++) {
        if (L[i] < R[i]) {
			return true;
		}
        if (L[i] > R[i]) {
			return false;
		}
    }
    if (sizeL < sizeR) {
		return true;
	}
    return false;
}

static void merge(char **A, char **L, int leftCount, char **R, int rightCount) {
	int	i = 0, j = 0, k =0;
	while (i < leftCount && j < rightCount) {
		if (isSmaller(L[i], R[j])) {
			A[k++] = L[i++];
		}
		else if (isSmaller(R[j], L[i])) {
			A[k++] = R[j++];
		}
        else {
            A[k++] = R[j++];
            free(L[i++]);
        }
	}
	while (i < leftCount) {
		A[k++] = L[i++];
	}
	while (j < rightCount) {
		A[k++] = R[j++];
	}
    while (k < i + j) {
		A[k++] = NULL;
	}
}

static void mergeSort(char **A, int n) {
	int mid, i;
    char **L, **R;
	if (n < 2) {
		return;
	}
	mid = n/2; 
	L = (char **)malloc(mid * sizeof(char *)); 
	R = (char **)malloc((n - mid) * sizeof(char *)); 
	for (i = 0; i < mid; i++) {
		L[i] = A[i];
	}
	for (i = mid; i < n; i++) {
		R[i - mid] = A[i];
	}
	mergeSort(L, mid);
	mergeSort(R, n - mid);
	merge(A, L, mid, R, n - mid);
    free(L);
    free(R);
}

struct PhoneNumbers const *phfwdReverse(struct PhoneForward *pf, char const *num) {
    if (!isNumber(num)) {
		return phnumNew(0);
	}
   
	// Zlicza, ile istnieje przekierowań na num
    ReverseTree *reverse = pf->reverse;
    int forwardCount = 1;
    int i = 0;
    while (num[i] != 0) {
        int digit = getDigit(num[i]);
        if (reverse->next[digit] == NULL) {
			break;
		}
        reverse = reverse->next[digit];
        forwardCount = forwardCount + reverse->forwardCount;
        i++;
    }

    struct PhoneNumbers *res;
	res = phnumNew(forwardCount);
    if (res == NULL) {
		return NULL;
	}

	// Wrzuca do struktury każde przekierowanie na num
	forwardCount--;
    res->list[forwardCount] = (char *)malloc(strlen(num) + 1);
    strcpy(res->list[forwardCount], num);

    while (reverse->prev != NULL) {
        List *current = reverse->from;
        while (current->next != NULL) {
            forwardCount--;
			res->list[forwardCount] = connect((current->next)->id, num, i);
            if (res->list[forwardCount] == NULL) {
                phnumDelete(res);
                return NULL;
            }
            current = current->next;
        }
        reverse = reverse->prev;
        i--;
    }

    mergeSort(res->list, res->size);
    return res;
}

char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL || (int)idx >= pnum->size) {
		return NULL;
	}
    return pnum->list[idx];
}

// Zwraca a^b, jeśli a dodatnie
// W przeciwnym przypadku zwraca 0
static size_t pot(size_t a, size_t b) {
	if (a <= 0) {
		return 0;
	}
	size_t i;
	size_t result = 1;
	size_t x = a;
	for (i = 1; i <= b; i<<=1) {
		if ((b&i) != 0) {
			result *= x;
		}
		x *= x;
	}
	return result;
} 

static size_t recPhfwdNonTrivialCount(ReverseTree *reverse, bool *possible, size_t possibleNum, size_t len) {
	if (strcmp(reverse->to, EMPTY) && strlen(reverse->to) > len) {
		return 0;
	}

    size_t res = 0;
	if (reverse->from->next != NULL) {
		size_t revLen = strlen(reverse->to);
		return pot(possibleNum, (len - revLen));
	}

	int i;
    for (i = 0; i < SIZE; i++) {
        if (possible[i] && reverse->next[i] != NULL) {
            res += recPhfwdNonTrivialCount(reverse->next[i], possible, possibleNum, len);
		}
	}
	return res;
}

size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) {
	if (pf == NULL || len == 0 || set == NULL ) {
		return 0;
	}

	bool possible[SIZE + 1];
	for (int i = 0; i <= SIZE; i++) {
		possible[i] = false;
	}
	int i = 0;
	while (set[i] != 0) {
		if (isDigit(set[i])) {
			possible[getDigit(set[i])] = true;
		}
		i++;
	}
	size_t possibleNum = 0;
	for (int i = 0; i <= SIZE; i++) {
		if (possible[i]) {
			possibleNum++;
		}
	}
	if(possibleNum == 0) {
		return 0;
	}
	return recPhfwdNonTrivialCount(pf->reverse, possible, possibleNum, len);
}
