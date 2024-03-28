#include "parent.h"

int main(int argc, char* argv[], char* envp[]) {

    setlocale(LC_COLLATE, "C");                                                      // Установить локаль.

    int i = 0;
    while (envp[i++]);                                                               // Узнать число параметров среды.
    qsort(envp, i-1, sizeof(char*), compare);                                        // Сортировка.
    i = 0;
    while(envp[i]) {
        printf("%s\n", envp[i++]);
    }

    char *childName = (char*)calloc(9, sizeof(char));
    strcpy(childName, "child_00");

    while(1) {
        char key = getchar();                                      
        while (getchar() != '\n')
            continue;
        if (key == 'q')
            break;
        else if (key == '+' || key == '*' || key == '&') {
            pid_t chpid = fork();                                                    // Новый процесс.
            if (chpid == 0) {
                char *childDir = NULL;
                switch (key) {                                                       // Получаем путь к программе сhild в соответствии с нажатой клавишей.
                    case '+':
                        childDir = getenv("CHILD_PATH");
                        break;
                    case '*':
                        childDir = scanEnvp(envp);
                        break;
                    case '&':
                        childDir = scanEnvp(environ);
                        break;
                }
                if (childDir) {
                    char *newArgv[] = {childName, argv[1], &key, NULL};
                    execve(childDir, newArgv, envp);                                // Вызов программы.
                } else {
                    printf("Error occured, path not find.\n");
                }
            } else if (chpid == -1) {
                printf("Error occured, error code - %d\n", errno);
                exit(errno);
            } else {
                incName(&childName);                                                // Увеличить номер процесса.
            }
        } else
            printf("Unknown key. Press '+', '*', '&' or q.\n");
        waitpid(-1, NULL, WNOHANG);                                                 // Ожидание завершения процесса.
    }

    free(childName);

    return 0;
}