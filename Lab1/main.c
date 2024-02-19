#include "header.h"

int l = 0;                                   // Символические ссылки.
int d = 0;                                   // Каталоги.
int f = 0;                                   // Файлы.
int s = 0;                                   // Сортировка по алфавиту.

NODE* list = NULL;                           // Список.

int main(int argc, char* argv[]) {
    scanparams(argc, argv);                  // Чтение параметров вывода.
    char *directory = scand(argc, argv);     // Чтение каталога.
    seedirs(directory);                      // Поиск данных.
    show(list);                              // Вывод на экран.
    clearList(&list);                  // Очистка списка.
    return 0;
}