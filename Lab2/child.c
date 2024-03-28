#include "child.h"

int main(int argc, char* argv[], char* envp[]) {

    printf("Process name: %s\nProcess pid: %d\nProcess ppdid: %d\n", argv[0], (int) getpid(), (int) getppid());

    char **variables = getShortVariables(argv[1]);                             // Получаем список необходимых переменных среды из файла.
    if (variables != NULL) {
        switch (argv[2][0]) {
            case '+':
                for (int i = 0; i < VAR_NUM; i++)                              // Получаем переменную среды системной функцией.
                    printf("%s=%s\n", variables[i], getenv(variables[i]));
                break;
            case '*':
                for (int i = 0; i < VAR_NUM; i++)                              // Получаем переменную среды из аргументов.
                    printf("%s\n", findEnvpVariable(envp, variables[i]));
                break;
            case '&':
                for (int i = 0; i < VAR_NUM; i++)                              // Получаем переменную среды из внешней переменной.
                    printf("%s\n", findEnvpVariable(environ, variables[i]));
                break;
        }
        for (int i = 0; i < VAR_NUM; i++)
            free(variables[i]);
    }
    free(variables);
    return 0;
}
