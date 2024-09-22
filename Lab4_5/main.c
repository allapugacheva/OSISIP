#include "mainFunctions.h"

int continuing = 1;

STACKNODE* stackFiller = NULL;
STACKNODE* stackExtractor = NULL;

key_t key1;
key_t key2;
int msqid;
int shmid;
COUNTER* counter;

int main() {
    srand(time(NULL));

    sem_unlink(FILL_SEM_NAME);
    sem_unlink(EXTRACT_SEM_NAME);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    signal(SIGUSR1, changeContinuingStatus);

    if ((key1 = ftok("/home/andrey/OSISP/Lab4_5/main.c", 'A')) == -1) {
        printf("Error ftok, %d.\n", errno);
        exit(errno);
    }

    if ((key2 = ftok("/home/andrey/OSISP/Lab4_5/main.c", 'B')) == -1) {
        printf("Error ftok, %d.\n", errno);
        exit(errno);
    }

    if ((msqid = msgget(key1, 0666 | IPC_CREAT)) == -1) {
        printf("Error while creating queue, code %d.\n", errno);
        exit(errno);
    }

    if ((shmid = shmget(key2, sizeof(COUNTER), 0666 | IPC_CREAT)) == -1) {
        printf("Error while creating sharing memory, code %d.\n", errno);
        exit(errno);
    }

    if ((counter = (COUNTER *) shmat(shmid, NULL, 0)) == (COUNTER *) -1) {
        printf("Error shmat.\n");
        exit(errno);
    }
    counter->countAdded = 0;
    counter->countDeleted = 0;

    if (shmdt(counter) == -1) {
        printf("Error shmdt.\n");
        exit(errno);
    }

    sem_t *fillSem;
    if ((fillSem = sem_open(FILL_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open filling semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t *extractSem;
    if ((extractSem = sem_open(EXTRACT_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open extracting semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t *queueAccess;
    if ((queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open queue semaphore, code %d.\n", errno);
        exit(errno);
    }

    while (continuing) {
        char ch = getchar();
        switch (ch) {
            case 'w': { // create filler
                pid_t pid = fork();
                if (pid == -1) {
                    printf("Error occurred while creating new filler, error code %d.\n", errno);
                    exit(errno);
                } else if (pid == 0) {
                    fillingMessages();
                    exit(0);
                } else
                    pushStack(&stackFiller, pid);
                break;
            }
            case 's': // kill filler
                if (stackFiller != NULL) {
                    kill(stackFiller->pid, SIGUSR1);
                    waitpid(stackFiller->pid, NULL, 0);
                    popStack(&stackFiller);
                } else
                    printf("There is no fillers.\n");
                break;
            case 'e': { // create extractor
                pid_t pid = fork();
                if (pid == -1) {
                    printf("Error occurred while creating new extractor, error code %d.\n", errno);
                    exit(errno);
                } else if (pid == 0) {
                    extractingMessages();
                    exit(0);
                } else
                    pushStack(&stackExtractor, pid);
                break;
            }
            case 'd':
                if (stackExtractor != NULL) {
                    kill(stackExtractor->pid, SIGUSR1);
                    waitpid(stackExtractor->pid, NULL, 0);
                    popStack(&stackExtractor);
                } else
                    printf("There is no extractors.\n");
                break;
            case 'q':
                while (stackFiller != NULL) {
                    kill(stackFiller->pid, SIGUSR1);
                    waitpid(stackFiller->pid, NULL, 0);
                    popStack(&stackFiller);
                }
                while (stackExtractor != NULL) {
                    kill(stackExtractor->pid, SIGUSR1);
                    waitpid(stackExtractor->pid, NULL, 0);
                    popStack(&stackExtractor);
                }
                printf("All fillers are extractors are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }

    msgctl(msqid, IPC_RMID, NULL);

    sem_close(fillSem);
    sem_unlink(FILL_SEM_NAME);
    sem_close(extractSem);
    sem_unlink(EXTRACT_SEM_NAME);
    sem_close(queueAccess);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    return 0;
}
