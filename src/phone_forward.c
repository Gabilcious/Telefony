#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "phone_forward.h"

struct PhoneForward * phfwdNew(void) {
    struct PhoneForward *element;
    if((element = (struct PhoneForward *)malloc(sizeof(PhoneForward))) == NULL
         || (element->forward = newNode(NULL, -1)) == NULL
         || (element->reverse = newNode(NULL, -1)) == NULL) return NULL;
    return element;
}

void phfwdDelete(struct PhoneForward *pf) {
    if(pf == NULL) return;
    removeAllNodes(pf->forward);
    removeAllNodes(pf->reverse);
    free(pf);
}

inline bool isNumber(char const *ch) {
    int i = 0;
    while(ch[i] != 0) if (!isdigit(ch[i++])) return false;
    return (strlen(ch)>0);
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {
    if(!isNumber(num1) || !isNumber(num2) || !strcmp(num1,num2)) return false;
    Tree *forward = pf->forward;
    Tree *reverse = pf->reverse;
    int i = 0;
    
    while(num1[i] != 0) {
        int x = (int)num1[i] - 48;
        if(forward->next[x] == NULL)
            if((forward->next[x] = newNode(forward, x)) == NULL) {
                clearFromNode(forward);
                return false;
            }
        forward = forward->next[x];
        i++;
    }
    i = 0;
    while(num2[i] != 0) {
        int x = (int)num2[i] - 48;
        if(reverse->next[x] == NULL)
            if((reverse->next[x] = newNode(reverse, x)) == NULL) {
                clearFromNode(forward);
                clearFromNode(reverse);
                return false;
            }
        reverse = reverse->next[x];
        i++;
    }
    char *numm2;
    numm2 = malloc(strlen(num2)*sizeof(char)+1);
    char *numm1;
    numm1 = malloc(strlen(num1)*sizeof(char)+1);
    strcpy(numm2,num2);
    strcpy(numm1,num1);
    free(forward->id);
    free(reverse->id);
    forward->id = numm2;
    reverse->id = numm1;
    return true;
}

void recPhfwdRemove(Tree *forward, Tree *reverse) {
    int i;
    for(i = 0; i < SIZE; i++)
        if(forward->next[i] != NULL) recPhfwdRemove(forward->next[i], reverse);
    removeConcreteNode(reverse, forward->id);
    removeNode(forward);
}

void phfwdRemove(struct PhoneForward *pf, char const *num) {
    if(num == NULL || strlen(num)==0) return;
    Tree *forward = pf->forward;
    int i = 0;
    while(num[i] != 0) {
        int x = (int)num[i] - 48;
        if(forward->next[x] == NULL) return;
        forward = forward->next[x];
        i++;
        
    }
    Tree *prev = forward->prev;
    recPhfwdRemove(forward, pf->reverse);
    clearFromNode(prev);
}

//DO POPRAWKI 2xCOPY
char * connect(char *first, char const *second, int secIndex){
    char *res;
    int firstLen = (int)strlen(first);
    int size = firstLen + (int)strlen(second) - secIndex;
    res = (char *)malloc(size*sizeof(char)+1);
    int i;
    for(i = 0; i < size; i++) {
        if (i < firstLen) res[i] = first[i];
        else res[i] = second[secIndex++];
    }
    return res;
}

struct PhoneNumbers * phnumNew(int size) {
    struct PhoneNumbers *element;
    if((element = (struct PhoneNumbers *)malloc(sizeof(PhoneNumbers))) == NULL) return NULL;
    if((element->list = (char **)malloc(size*sizeof(char *))) == NULL) {
        free(element);
        return NULL;
    }
    element->size = size;
    int i;
    for(i = 0; i < size; i++) element->list[i] = NULL;
    return element;
}


struct PhoneNumbers const * phfwdGet(struct PhoneForward *pf, char const *num) {
    if(!isNumber(num)) return phnumNew(0);
    struct PhoneNumbers *res;
    if((res = phnumNew(1)) == NULL) return NULL;
    Tree *forward = pf->forward;
    int i = 0;
    while(num[i] != 0) {
        int x = (int)num[i] - 48;
        if(forward->next[x] == NULL) break;
        forward = forward->next[x];
        i++;
    }
    while(forward->x != -1) {
        if(strcmp(forward->id,EMPTY)) {
            if((res->list[0] = connect(forward->id, num, i)) == NULL) {
                phnumDelete(res);
                return NULL;
            }
            return res;
        }
        forward = forward->prev;
        i--;
    }
    char *c;
    c = (char *)malloc(strlen(num)*sizeof(char)+1);
    strcpy(c,num);
    res->list[0] = c;
    return res;
}

bool isSmaller(char *L, char *R) {
    if(L == NULL) return false;
    if(R == NULL) return true;
    int sizeL = strlen(L);
    int sizeR = strlen(R);
    int i;
    for(i = 0; i < sizeL && i < sizeR; i++) {
        if(L[i] < R[i]) return true;
        if(L[i] > R[i]) return false;
    }
    if(sizeL < sizeR) return true;
    return false;
}

void Merge(char **A, char **L, int leftCount, char **R, int rightCount) {
	int	i = 0, j = 0, k =0;
	while(i<leftCount && j< rightCount) {
		if(isSmaller(L[i],R[j])) A[k++] = L[i++];
		else if (isSmaller(R[j],L[i])) A[k++] = R[j++];
        else {
            A[k++] = R[j++];
            free(L[i++]);
        }
	}
	while(i < leftCount) A[k++] = L[i++];
	while(j < rightCount) A[k++] = R[j++];
    while(k < i + j) A[k++] = NULL;
}
 
void MergeSort(char **A, int n) {
	int mid, i;
    char **L, **R;
	if(n < 2) return; 
	mid = n/2; 
	L = (char **)malloc(mid*sizeof(char *)); 
	R = (char **)malloc((n-mid)*sizeof(char *)); 
	for(i = 0;i<mid;i++) L[i] = A[i];
	for(i = mid;i<n;i++) R[i-mid] = A[i];
	MergeSort(L,mid);
	MergeSort(R,n-mid);
	Merge(A,L,mid,R,n-mid);
        /////czy muszę zwalniać pojedyncze elementy tablicy forem? nie mallocowałam ich
    free(L);
    free(R);
}

struct PhoneNumbers const * phfwdReverse(struct PhoneForward *pf, char const *num) {
    if(!isNumber(num)) return phnumNew(0);
    int size = 1;
    Tree *reverse = pf->reverse;
    int i = 0;
    while(num[i] != 0) {
        int x = (int)num[i] - 48;
        if(reverse->next[x] == NULL) break;
        reverse = reverse->next[x];
        if (strcmp(reverse->id,EMPTY)) {
            size++;
        }
        i++;
    }
    struct PhoneNumbers *res;
    if((res = phnumNew(size)) == NULL) return NULL;
    char *c;
    c = (char *)malloc(strlen(num)*sizeof(char)+1);
    strcpy(c,num);
    res->list[--size] = c;
    while(reverse != pf->reverse) {
        if(strcmp(reverse->id,EMPTY)) {
            if((res->list[--size] = connect(reverse->id, num, i)) == NULL) {
                phnumDelete(res);
                return NULL;
            }
        }
        reverse = reverse->prev;
        i--;
    }
    MergeSort(res->list,res->size);
    return res;
}

void pisz(char *c) {
    int i = 0;
    while(c[i]!=0) printf("%c",c[i++]);
    printf("\n");
}

char const * phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
    /** if(pnum == NULL || (int)idx >= pnum->size) return NULL; */
    printf("%s\n",pnum->list[idx]);
    /** return pnum->list[idx]; */
    return "123";
}
