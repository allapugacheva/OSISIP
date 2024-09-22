#pragma once
#include "ring.h"
#include "stack.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>

#define FILL_SEM_NAME "/fill_sem"
#define EXTRACT_SEM_NAME "/extract_sem"
#define QUEUE_ACCESS_SEM_NAME "/queue_sem"

extern STACKNODE* stackFiller;
extern STACKNODE* stackExtractor;

extern int continuingFiller;
extern int continuingExtractor;
extern QUEUE* queue;

extern pthread_mutex_t mutex;
extern pthread_cond_t condFill;
extern pthread_cond_t condExtract;

void *fillingMessages(void* args);
void *extractingMessages(void* args);

void stopFilling();
void stopExtracting();