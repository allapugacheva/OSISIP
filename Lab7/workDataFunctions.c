#include "workDataFunctions.h"

int n = -1;
record_s* dataCopy = NULL;

void listData() {
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_len = 0;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while making block.\n");
        close(fd);
        exit(errno);
    }

    lseek(fd, 0, SEEK_SET);

    int i = 1;
    record_s temp;
    while(read(fd, &temp, sizeof(record_s))>0)
        printf("Record %d: name - %s, address - %s, semester - %hhu.\n", i++, temp.name, temp.address, temp.semester);

    fl.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while breaking block.\n");
        close(fd);
        exit(errno);
    }
}
record_s *getNEl() {
    printf("Enter N: ");
    scanf("%d", &n);
    while(getchar()!='\n')
        continue;

    record_s *temp = (record_s *) malloc(sizeof(record_s));

    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_len = sizeof(record_s);
    fl.l_start = (n-1)*sizeof(record_s);
    fl.l_whence = SEEK_SET;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while making block.\n");
        close(fd);
        exit(errno);
    }

    lseek(fd, (n-1)*sizeof(record_s), SEEK_SET);

    if(read(fd, temp, sizeof(record_s))==0) {
        printf("Error while reading N record.\n");
        close(fd);
        exit(errno);
    }

    fl.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while breaking block.\n");
        close(fd);
        exit(errno);
    }

    if(dataCopy!=NULL)
        free(dataCopy);
    dataCopy = (record_s*)malloc(sizeof(record_s));
    strcpy(dataCopy->name, temp->name);
    strcpy(dataCopy->address, temp->address);
    dataCopy->semester = temp->semester;

    printf("Record: name - %s, address - %s, semester - %hhu.\n", temp->name, temp->address, temp->semester);
    printf("Edit?\n1) Yes\n2) No:");
    if(getchar()=='1') {
        while(getchar()!='\n')
            continue;
        changeNEl(temp);
    }
    while(getchar()!='\n')
        continue;

    return temp;
}
void changeNEl(record_s* data) {
    if(data==NULL) {
        printf("Data is empty, enter data.\n");
        return;
    }

    printf("Change record: name - %s, address - %s, semester - %hhu.\n", data->name, data->address, data->semester);

    int continuing = 1;
    while(continuing) {
        printf("Change:\n1) Name\n2) Address\n3) Semester\n4) Done:");
        char ch = getchar();
        while(getchar()!='\n')
            continue;
        switch(ch) {
            case '1':
                printf("Enter name: ");
                fgets(data->name, 80, stdin);
                data->name[strlen(data->name)-1] = '\0';
                break;
            case '2':
                printf("Enter address: ");
                fgets(data->address, 80, stdin);
                data->address[strlen(data->address)-1] = '\0';
                break;
            case '3':
                printf("Enter semester: ");
                scanf("%hhu", &data->semester);
                while(getchar()!='\n')
                    continue;
                break;
            case '4':
                continuing = 0;
                break;
        }
    }

    printf("Save changes?\n1) Yes\n2) No;");
    if(getchar()=='1') {
        while(getchar()!='\n')
            continue;
        saveNEl(data);
    }
    while(getchar()!='\n')
        continue;
}
void saveNEl(record_s* data) {
    if(data==NULL) {
        printf("Data is empty, enter data.\n");
        return;
    }

    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_len = sizeof(record_s);
    fl.l_start = (n-1)*sizeof(record_s);
    fl.l_whence = SEEK_SET;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while making block.\n");
        close(fd);
        exit(errno);
    }

    lseek(fd, (n-1)*sizeof(record_s), SEEK_SET);

    record_s *temp = (record_s *) malloc(sizeof(record_s));
    if(read(fd, temp, sizeof(record_s))==0) {
        printf("Error while rereading N record.\n");
        close(fd);
        exit(errno);
    }

    fl.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while breaking block.\n");
        close(fd);
        exit(errno);
    }

    if(strcmp(dataCopy->name, temp->name)!=0 || strcmp(dataCopy->address, temp->address)!=0 || dataCopy->semester!=temp->semester) {

        printf("Data was changed, current data: \n");
        printf("Record: name - %s, address - %s, semester - %hhu.\n", temp->name, temp->address, temp->semester);
        data = temp;

        strcpy(dataCopy->name, temp->name);
        strcpy(dataCopy->address, temp->address);
        dataCopy->semester = temp->semester;

        changeNEl(data);
        return;
    }

    fl.l_type = F_WRLCK;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while making block.\n");
        close(fd);
        exit(errno);
    }

    lseek(fd, (n-1)*sizeof(record_s), SEEK_SET);

    if(write(fd, data, sizeof(record_s))==0) {
        printf("Error while writing N record.\n");
        close(fd);
        exit(errno);
    }

    fl.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLKW, &fl) == -1) {
        printf("Error while breaking block.\n");
        close(fd);
        exit(errno);
    }

    printf("Changes saved.\n");
}