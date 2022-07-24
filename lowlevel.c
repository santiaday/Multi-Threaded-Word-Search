
#include "buff.h"

int lowlvl(int linesize, int dim, int row, int column, FILE* fd, char** buffer){
    int i = 0;
    long initial = row * (linesize+1) + column;
    printf("Initial index: #%d\n", initial);
    for(i = 0;i < dim;i++)
        buffer[i] = (char*)malloc(dim);
    lseek(fd, initial,SEEK_SET);
    for(i = 0; i < dim;i++){
        read(fd, buffer[i], dim);
        lseek(fd, linesize-dim+1, SEEK_CUR);
    }

}