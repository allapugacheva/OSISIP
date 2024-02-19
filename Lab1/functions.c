#include "header.h"

// Добавление новой папки к текущей строке с каталогом.
const char* adddir(const char* cur, const char* new) {
    char *res = (char *) malloc(strlen(cur) + strlen(new) + 2); // Выделение места.
    for (int i = 0; i < strlen(cur); i++)                               // Копирование текущего каталога.
        res[i] = cur[i];
    res[strlen(cur)] = '/';
    for (int i = 0; i < strlen(new); i++)                               // Копирование новой папки.
        res[strlen(cur) + 1 + i] = new[i];
    res[strlen(cur) + strlen(new) + 1] = '\0';
    return res;
}
// Просмотр содержимого.
void seedirs(const char* directory) {
    DIR *dir = opendir(directory);                   // Открыть каталог.
    if (!dir) {                                            // Проверка успешности открытия.
        perror("opendir");
        exit(errno);
    }
    struct dirent *curdir;                                 // Структура с информацией о файлах каталога.
    for (;;) {                                             // Цикл по всем файлам.
        curdir = readdir(dir);                        // Чтение следующего файла/каталога/символической ссылки.
        if (curdir == NULL) {                              // Проверка на ошибку/конец каталога.
            if (errno) {                                   // Ошибка?
                perror("readdir");
                exit(errno);
            }
            break;                                         // Конец каталога.
        }
        switch (curdir->d_type) {                          // Обработка типов.
            case DT_DIR:                                   // Каталог.
                if (strcmp(curdir->d_name, ".") != 0 && strcmp(curdir->d_name, "..") != 0) {
                    if (d) {                               // Проверка, что нужно вывести каталоги.
                        if (s)                             // Проверка, что нужно в алфавитном порядке.
                            addAsc(&list, adddir(directory, curdir->d_name));
                        else
                            addEnd(&list, adddir(directory, curdir->d_name));
                    }                                      // Рекурсивный переход к новому каталогу.
                    seedirs(adddir(directory, curdir->d_name));
                }
                break;
            case DT_LNK:                                   // Символическая ссылка.
                if (l){
                    if (s)
                        addAsc(&list, adddir(directory, curdir->d_name));
                    else
                        addEnd(&list, adddir(directory, curdir->d_name));
                }
                break;
            case DT_REG:                                   // Файл.
                if (f) {
                    if (s)
                        addAsc(&list, adddir(directory, curdir->d_name));
                    else
                        addEnd(&list, adddir(directory, curdir->d_name));
                }
                break;
        }
    }
    closedir(dir);                                    // Закрыть каталог.
}
// Получение параметров из командной строки.
void scanparams(int argc, char* argv[]) {
    int rez = 0;                                           // Получение следующего параметра.
    while ((rez = getopt(argc, argv, "ldfs")) != -1) {
        switch (rez) {                                     // Проверка на наличие одного из символов.
            case 'l':
                l = 1;
                break;
            case 'd':
                d = 1;
                break;
            case 'f':
                f = 1;
                break;
            case 's':
                s = 1;
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                exit(0);
        }
    }
    if (!l && !d && !f) {                                  // Проверка, что ни один символ не найден - вывести все типы файлов.
        l = 1;
        d = 1;
        f = 1;
    }
}
// Получение каталога из командной строки.
char* scand(int argc, char* argv[]) {
    char *directory = ".";                                 // По умолчанию - текущий каталог.
    for (int i = 1; i < argc; i++)
        if (argv[i][0] != '-') {                           // Проверка, что введён каталог.
            directory = argv[i];
            break;
        }
    if (d) addEnd(&list, directory);                       // Вывод текущего каталога.
    return directory;
}