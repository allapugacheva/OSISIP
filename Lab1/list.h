#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node                   // Узел списка.
{
    char* str;
    struct node* next;
    struct node* prev;
} NODE;

NODE* makeNode(const char*);          // Создать новый узел.

void addEnd(NODE**, const char*);     // Добавление.
void addAsc(NODE**, const char*);
void show(NODE*);                     // Вывод.
void clearList(NODE** head);          // Очистка.