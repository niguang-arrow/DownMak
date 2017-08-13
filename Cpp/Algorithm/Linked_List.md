# 链表

## 2017 年 8 月 13 日

+ 今日开始学习链表


```c
// list.h

#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);

#endif


// list.c

#include "list.h"

struct Node {
    ElementType Element;
    Position Next;
};

int IsEmpty(List L) {
    return L->Next == NULL;
}

int IsLast(Position P, List L) {
    return P->Next == NULL;
}

Position
Find(ElementType X, List L) {
    Position P = L->Next;
    while (P != NULL && X != P->Element)
        P = P->Next;
    return P;
}

void
DeleteList(List L) {
    Position P, tmp;
    P = L->Next;
    L->Next = NULL;
    while (P != NULL) {
        tmp = P->Next;
        free(P);
        P = tmp;
    }
}

/*
 * Insert after legal Position P
 * */
void Insert(ElementType X, List L, Position P) {
    Position TmpCell;
    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL) {
        fprintf(stderr, "Malloc is failed...\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    TmpCell->Element = X;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
}

void Delete(ElementType X, List L) {
    Position P = FindPrevious(X, L);
    Position TmpCell;
    if (!IsLast(P, L)) {
       TmpCell = P->Next;
       P->Next = TmpCell->Next; 
       free(TmpCell);
    }
}

Position
FindPrevious(ElementType X, List L) {
    Position P = L;
    while (P->Next != NULL && X != P->Element)
        P = P->Next;
    return P;
}
```

+ 当然还有一些函数并没有实现, 靠你了.
