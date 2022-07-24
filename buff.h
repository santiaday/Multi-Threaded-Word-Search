#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int size;
    int thread_id;
    int col_no;
    int row_no;
    char** buffer;
}puzzle;

int lowlvl(int linesize, int dim, int row, int column, FILE* fd,char** buffer);
void* createPuzzle(void *passedPuzzle);