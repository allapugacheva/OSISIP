#include "list.h"

// Создание нового узла.
NODE* makeNode(const char* str) {
    NODE *temp = (NODE *) malloc(sizeof(NODE));       // Выделение места.
    temp->next = NULL;
    temp->prev = NULL;
    str == NULL ? (temp->str = NULL) : (temp->str = str);  // Присваивание данных.
    return temp;
}
// Добавление в конец списка.
void addEnd(NODE** head, const char* str) {
    if (*head == NULL)
        *head = makeNode(str);                  // Создание элемента, если список пуст.
    else {
        NODE *temp = *head;
        while (temp->next != NULL)              // Переход в конец.
            temp = temp->next;
        temp->next = makeNode(str);             // Создание узла.
        temp->next->prev = temp;
    }
}
// Добавление в алфавитном порядке.
void addAsc(NODE** head, const char* str) {
    if (*head == NULL)
        *head = makeNode(str);                  // Создание элемента, если список пуст.
    else {
        NODE *temp = *head;                     // Переход к нужному месту.
        while (temp->next != NULL && strcasecmp(temp->str, str) < 0) {
            temp = temp->next;
        }                                       // Проверка, что нужно вставить в конец.
        if (temp->next == NULL && strcasecmp(temp->str, str) < 0) {
            temp->next = makeNode(str);
            temp->next->prev = temp;
        } else {
            NODE *temp1 = makeNode(str);        // Временный элемент.
            temp1->next = temp;                 // Вставка.
            temp1->prev = temp->prev;
            temp->prev = temp1;
            if (temp1->prev != NULL)
                temp1->prev->next = temp1;
            else
                *head = (*head)->prev;          // Сдвиг головы, если вставка в начало.
        }
    }
}
// Вывод списка на экран.
void show(NODE* head) {
    NODE *temp = head;
    while (temp != NULL) {                      // Цикл по всем элементам.
        printf("%s\n", temp->str);
        temp = temp->next;
    }
}
// Очистка списка.
void clearList(NODE** head) {
    while (*head != NULL) {                     // Цикл по всем элементам.
        if ((*head)->next != NULL) {            // Не последний элемент.
            *head = (*head)->next;
            free((*head)->prev);            // Очистка.
        } else {
            free(*head);                    // Очистка головы.
            *head = NULL;
        }
    }
}