#include "mainFunctions.h"

int continuingFiller = 1;
int continuingExtractor = 1;
int MAX_MES_COUNT = 20;

STACKNODE* stackFiller = NULL;
STACKNODE* stackExtractor = NULL;

pthread_mutex_t mutex;
pthread_cond_t condFill;
pthread_cond_t condExtract;

QUEUE* queue;

int main() {
    srand(time(NULL));

    signal(SIGUSR1, stopFilling);
    signal(SIGUSR2, stopExtracting);

    queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->ringHead = 0;
    queue->ringTail = 0;
    queue->countDeleted = 0;
    queue->countAdded = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condFill, NULL);
    pthread_cond_init(&condExtract, NULL);

    int continuing = 1;
    while(continuing) {
        char ch = getchar();
        switch (ch) {
            case 'w': { // create filler
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, fillingMessages, (void*)&threadId) != 0) {
                    printf("Error while creating filling thread.");
                    continuing = 0;
                } else {
                    pushStack(&stackFiller, threadId);
                    printf("Filler created.\n");
                }
                break;
            }
            case 's': // kill filler
                if(stackFiller!=NULL) {
                    pthread_kill(stackFiller->threadId, SIGUSR1);
                    popStack(&stackFiller);
                    printf("Filler deleted.\n");
                } else
                    printf("There is no fillers.\n");
                break;
            case 'e': { // create extractor
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, extractingMessages, (void*)threadId) != 0) {
                    printf("Error while creating extracting thread.");
                    continuing = 0;
                } else {
                    pushStack(&stackExtractor, threadId);
                    printf("Extractor created.\n");
                }
                break;
            }
            case 'd':
                if(stackExtractor!=NULL) {
                    pthread_kill(stackExtractor->threadId, SIGUSR2);
                    popStack(&stackExtractor);
                    printf("Extractor deleted.\n");
                    continuingExtractor = 1;
                } else
                    printf("There is no extractors.\n");
                break;
            case '+':
                MAX_MES_COUNT++;
                break;
            case '-':
                MAX_MES_COUNT--;
                break;
            case 'q':
                while(stackFiller != NULL) {
                    pthread_cancel(stackFiller->threadId);
                    popStack(&stackFiller);
                }
                while(stackExtractor != NULL) {
                    pthread_cancel(stackExtractor->threadId);
                    popStack(&stackExtractor);
                }
                printf("All fillers are extractors are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }

    pthread_cond_destroy(&condFill);
    pthread_cond_destroy(&condExtract);

    return 0;
}