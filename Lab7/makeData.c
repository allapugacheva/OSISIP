#include "data.h"
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 20

int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "ab+");
        if (f == NULL) {
            printf("Error while open file.\n");
            return 0;
        }

        record_s temp;
        while (1) {
            printf("Enter name: ");
            fgets(temp.name, 80, stdin);
            temp.name[strlen(temp.name) - 1] = '\0';
            printf("Enter address: ");
            fgets(temp.address, 80, stdin);
            temp.address[strlen(temp.address) - 1] = '\0';
            printf("Enter semester: ");
            scanf("%hhu", &temp.semester);

            fwrite(&temp, sizeof(record_s), 1, f);

            while (getchar() != '\n')
                continue;
            printf("Enter 1 to continue: ");
            if (getchar() != '1')
                break;
            while (getchar() != '\n')
                continue;
        }

        fclose(f);
    } else
        printf("Enter name of file.\n");
    return 0;
}