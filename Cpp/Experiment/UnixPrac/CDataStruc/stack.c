#include <stdio.h>
#include <stdlib.h>

typedef int Dtype;


typedef struct stack {
    int Capacity;
    Dtype top;
    Dtype *data;
} stack;

void* emalloc(int size) {
    void *ptr;
    ptr = malloc(size * sizeof(Dtype));
    if (ptr == NULL) {
        perror("Memory Allocation");
        exit(1);
    }
    return ptr;
}

void initStack(stack *sta, int cap) {
    sta->Capacity = cap;
    sta->data = (Dtype*)emalloc(cap);
    sta->top = -1;
}


int size(stack *sta) {
    return sta->top + 1;
}

int isEmpty(stack *sta) {
    return sta->top == -1;
}

int isFull(stack *sta) {
    return sta->top == sta->Capacity;
}

void push(stack *sta, Dtype elem) {
    if (isFull(sta)) {
        fprintf(stderr, "Stack is full\n");
        exit(1);
    }
    sta->data[++sta->top] = elem;
}

void pop(stack *sta) {
    if (isEmpty(sta)) {
        fprintf(stderr, "Stack is empty\n");
        exit(1);
    }
    sta->top--;
}

Dtype peek(stack *sta) {
    if (isEmpty(sta)) {
        fprintf(stderr, "Stack is empty\n");
        exit(1);
    }
    return sta->data[sta->top];
}


int main() {
    stack sta;
    initStack(&sta, 5);
    int i;
    for (i = 0; i < 5; ++i) {
        push(&sta, i + 5);
    }

    for (i = 0; i < 5; ++i) {
        printf("%d ", peek(&sta));
        pop(&sta);
    }

    printf("\n");
    free(sta.data);
    return 0;
}
