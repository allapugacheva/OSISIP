#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int num;
    pid_t pid;
    struct node* next;
} NODE;

void push(NODE**, pid_t);
void pop(NODE**);
void show(NODE*);
void clear(NODE**);
int size(NODE*);