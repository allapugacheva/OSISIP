#include "parent.h"

int includeString(const char* str1, const char* str2) {
    for(size_t i = 0; i<strlen(str2); i++)
        if(str1[i]!=str2[i])
            return 0;
    return 1;
}

char* scanEnvp(char* envp[]) {
    int i = 0;
    while(envp[i]) {
        if(includeString(envp[i], "CHILD_PATH"))                           // Обход всех параметров и проверка на совпадение.
            break;
        i++;
    }
    if(envp[i]==NULL)
        return NULL;
    char* result = (char*)calloc(strlen(envp[i]) - 10, sizeof(char));
    for(size_t j = 0; j<strlen(envp[i]); j++)                                 
        result[j] = envp[i][j+11];
    result[strlen(envp[i])-11] = '\0';
    return result;
}

int compare(const void *a, const void *b) {
    return strcoll(*(const char **)a, *(const char **)b);
}

void incName(char** name) {
    if ((*name)[7] == '9') {
        (*name)[6]++;
        (*name)[7] = 0;
    } else
        (*name)[7]++;
}