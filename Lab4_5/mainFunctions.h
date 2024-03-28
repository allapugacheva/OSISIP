#pragma once
#include <time.h>
#include "stack.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define FILL_SEM_NAME "/fill_sem"
#define EXTRACT_SEM_NAME "/extract_sem"
#define QUEUE_ACCESS_SEM_NAME "/queue_sem"

typedef struct {
    long mtype;
    //u_int8_t mtype;
    u_int16_t hash;
    u_int8_t size;
    u_int8_t data[256];
} MESSAGE;

typedef struct {
    int countAdded;
    int countDeleted;
} COUNTER;

#define MAX_MES_COUNT 20

extern int continuing;
extern key_t key1;
extern key_t key2;
extern int msqid;
extern int shmid;
extern COUNTER* counter;

void fillingMessages();
void extractingMessages();

void changeContinuingStatus();
MESSAGE* initMes();
void printMes(MESSAGE*);