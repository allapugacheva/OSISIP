#pragma once
#include "list.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

extern int l;                                    // Символические ссылки.
extern int d;                                    // Каталог.
extern int f;                                    // Файл.
extern int s;                                    // Упорядочить по алфавиту.

extern NODE* list;                               // Список содержимого.

const char* adddir(const char*, const char*);    // Добавление к текущей строке каталога новой папки.
void seedirs(const char*);                       // Просмотр содержимого.
void scanparams(int, char**);                    // Сканирование параметров.
char* scand(int, char**);                        // Сканирование каталога.
