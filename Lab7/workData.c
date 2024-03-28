#include "workDataFunctions.h"

int fd;

int main(int argc, char* argv[]) {
    if(argc>1) {
        if((fd = open(argv[1], O_RDWR | O_CREAT, 0666))== -1) {
            printf("Error while open file.\n");
            return 0;
        }

        record_s* temp = NULL;
        int continuing = 1;
        while(continuing) {
            printf("1) List file data.\n2) Get N element\n3) Change N element\n4) Save edited element\n5) Exit:");
            char ch = getchar();
            while(getchar()!='\n')
                continue;
            switch(ch) {
                case '1':
                    listData();
                    break;
                case '2':
                    temp = getNEl();
                    break;
                case '3':
                    changeNEl(temp);
                    break;
                case '4':
                    saveNEl(temp);
                    break;
                case '5':
                    continuing = 0;
                    break;
            }
        }

        close(fd);
    } else
        printf("Enter file name.\n");
    return 0;
}