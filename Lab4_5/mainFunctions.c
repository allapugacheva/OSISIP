#include "mainFunctions.h"

void fillingMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* fillSem = sem_open(FILL_SEM_NAME, 0);
    if(fillSem == SEM_FAILED) {
        printf("Error while opening filling semaphore.\n");
        exit(EXIT_FAILURE);
    }
    while (continuing) {
        sem_wait(queueAccess);
        sem_wait(fillSem);

        if((counter = (COUNTER*)shmat(shmid, NULL, 0))== (COUNTER*)-1) {
            printf("Error shmat.\n");
            exit(errno);
        }

        if (counter->countAdded - counter->countDeleted < MAX_MES_COUNT) {
            MESSAGE* msg = initMes();
            if(msgsnd(msqid, msg, sizeof(MESSAGE), IPC_NOWAIT | MSG_NOERROR) == -1) {
                printf("Error while sending message, %d\n", errno);
                continuing = 0;
            } else {
                counter->countAdded++;
                printf("Added %d message:\n", counter->countAdded);
                printMes(msg);
            }
            free(msg);
        } else
            printf("Queue is full!\n");

        if(shmdt(counter) == -1) {
            printf("Error shmdt.\n");
            exit(errno);
        }

        sem_post(fillSem);
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);
    sem_close(fillSem);
}
void extractingMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* extractSem = sem_open(EXTRACT_SEM_NAME, 0);
    if(extractSem == SEM_FAILED) {
        printf("Error while opening extracting semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while(continuing) {
        sem_wait(queueAccess);
        sem_wait(extractSem);

        if((counter = (COUNTER*)shmat(shmid, NULL, 0))== (COUNTER*)-1) {
            printf("Error shmat.\n");
            exit(errno);
        }

        if(counter->countAdded - counter->countDeleted > 0) {
            counter->countDeleted++;
            MESSAGE *msg = (MESSAGE*)malloc(sizeof(MESSAGE));
            if(msgrcv(msqid, msg, sizeof(MESSAGE), 1, 0) == -1) {
                printf("Error while extracting message.\n");
                continuing = 0;
            } else {
                printf("Deleted %d message:\n", counter->countDeleted);
                printMes(msg);
            }
            free(msg);
        } else
            printf("Queue is empty!\n");

        if(shmdt(counter) == -1) {
            printf("Error shmdt.\n");
            exit(errno);
        }

        sem_post(extractSem);
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);
    sem_close(extractSem);
}

void changeContinuingStatus() {
    continuing ^= 1;
}

MESSAGE* initMes() {
    MESSAGE* message = (MESSAGE*)malloc(sizeof(MESSAGE));
    message->mtype = 1;
    //message->type = 1;
    message->hash = 0;
    message->size = rand() % 257;
    for (size_t i = 0; i < message->size; i++) {
        message->data[i] = rand() % 256;
        message->hash += message->data[i];
    }
    message->hash = ~message->hash;
    return message;
}
void printMes(MESSAGE* mes) {
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->mtype, mes->hash, mes->size);
    for(size_t i = 0; i<mes->size; i++)
        printf("%d", mes->data[i]);
    printf("\n");
}