#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

int includeString(const char* str1, const char* str2);     // Включает ли подростроку.
char* scanEnvp(char* envp[]);                              // Получение пути к программе сhild.
int compare(const void *a, const void *b);                 // Сравнение двух объектов для qsort.
void incName(char** name);                                 // Увеличить номер процесса.