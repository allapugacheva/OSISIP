#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

extern int n;
extern record_s * dataCopy;
extern int fd;

void listData();
record_s *getNEl();
void changeNEl(record_s*);
void saveNEl(record_s*);