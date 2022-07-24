#include "buff.h"
#define START int main(int argc, char** argv){
#define END }
int dictionary_size;
char* dict[100000];
char* filename = NULL;
char* dict_filename;
pthread_t thread[200];
pthread_mutex_t mutex;                                                     
pthread_cond_t cond_available;
int len;

int binsearch(char* dictionaryWords[],int listSize,char* keyword){
    int mid, low = 0, high = listSize - 1;
    while (high >= low) {
        mid = (high + low) / 2;
        if (strcmp(dictionaryWords[mid], keyword) < 0)
            low = mid + 1;
        else if (strcmp(dictionaryWords[mid], keyword) > 0)
            high = mid - 1;
        else {
            return mid;
        }

    }
    return -1; 
}
void makeDic(char* dict_filename){
    FILE* f = fopen(dict_filename, "r");
    char temp[1000], line[1000];
    dictionary_size = 0;
    while(fgets(line, 1000, f)){
        sscanf(line, "%s\n", temp);
        if(strlen(temp) == 0)
            continue;
        dict[dictionary_size] = (char*) malloc(sizeof(temp)+1);
        strcpy(dict[dictionary_size++], temp);
    }
    fclose(f);
}
void* puzzleBoard(void *passedPuzzle){
    int column = 0, linesize = 0, size = 0, i = 0, fd, j, k;
    long row = 0;

    puzzle* my_puzzle = passedPuzzle;

    char* given_word;
    given_word = (char*)malloc(len+1);
    given_word[len] = '\0';


    for(i = 0; i < my_puzzle->size;i++)
        for(j = 0; j <= my_puzzle->size - len;j++){
            for(k = 0; k < len;k++){
                if(my_puzzle->buffer[i][k+j] == NULL)
                    continue;
                given_word[k] = my_puzzle->buffer[i][k+j];
            }

            if(binsearch(dict, dictionary_size, given_word) + 1)
                printf("Horizontally Matched Word: %s found by thread #%d\n", given_word,my_puzzle->thread_id);
            for(k = 0; k < len;k++){
                if(my_puzzle->buffer[k+j][i] == NULL)
                    continue;
                given_word[k] = my_puzzle->buffer[k+j][i];
            }
            if(binsearch(dict, dictionary_size, given_word) + 1)
                printf("Vertically Matched Word: %s found by thread #%d\n", given_word,my_puzzle->thread_id);
        }

    for(i = 0; i <= my_puzzle->size-len;i++)
        for(j = 0; j <=my_puzzle->size-len;j++){

            for(k = 0; k < len;k++){
                if(my_puzzle->buffer[i+k][j+k] == NULL)
                    continue;
                given_word[k] = my_puzzle->buffer[i+k][j+k];
            }
            if(binsearch(dict, dictionary_size, given_word) + 1)
                printf("Diagonally (LR) Matched Word: %s found by thread #%d\n", given_word,my_puzzle->thread_id);

            for(k = 0; k < len;k++){
                if(my_puzzle->buffer[i+k][j+len-1-k] == NULL)
                    continue;
                given_word[k] = my_puzzle->buffer[i+k][j+len-1-k];
            }

            if(binsearch(dict, dictionary_size, given_word) + 1)
                printf("Diagonally (RL) Matched Word: %s found by thread #%d\n", given_word,my_puzzle->thread_id);



        }
        printf("Thread #%d finished\n",my_puzzle->thread_id);
}

START
    int column = 0, linesize = 0, size = 0, i = 0, fd, j, k;
    long row = 0;

    char** buffer;
    while(++i < argc){
        if(argv[i][0] != '-'){
            fprintf(stderr, "Error: invalid CLA.\n");
            exit(1);
        }

        else if(!strcmp(argv[i], "-size"))
            size = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-input")){
            filename = (char*)malloc(1 + strlen(argv[++i]));
            strcpy(filename, argv[i]);
        }else if(!strcmp(argv[i], "-dict")){
            dict_filename = (char*)malloc(1 + strlen(argv[++i]));
            strcpy(dict_filename, argv[i]);
        }else if(!strcmp(argv[i], "-len"))
            len = atoi(argv[++i]);
        else{
            fprintf(stderr, "Error: invalid CLA.\n");
            exit(1);
        }
    }
    linesize = size;//idk why just works
    if(filename == NULL || size * linesize == 0){
        fprintf(stderr, "Error: invalid CLA.");
        exit(1);
    }
   
    long start = row * (linesize+1) + column;
    if((fd = open(filename, O_RDONLY, 0)) == -1){
        fprintf(stderr, "Error: can't open the file %s.", filename);
        exit(1);
    }

    makeDic(dict_filename);
    int r = 0;
    int c = 0;
    int buffer_len = 8000;
    int thread_no = 0;
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex, NULL);                            
    pthread_cond_init(&cond_available, NULL);
    for(r = 0; r < linesize; r+=(buffer_len - len))
        for(c = 0; c < linesize; c+= (buffer_len - len)){


            buffer = (char**)malloc(8000 * sizeof(char*));


            lowlvl(linesize,8000,r,c,fd,buffer);


            puzzle * my_puzzle = (puzzle *) malloc(sizeof(puzzle));
            my_puzzle -> buffer = buffer;
            my_puzzle -> row_no = r;
            my_puzzle -> col_no = c;
            my_puzzle -> thread_id = thread_no;
            my_puzzle->size = 8000;

            pthread_create(thread_no + thread,NULL,puzzleBoard,(void*)my_puzzle);
            thread_no++;
        }

    for(i = 0; i < thread_no; i++){
        printf("Thread ID #%d joined\n",i);
        pthread_join(thread[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    printf("Terminating Program...\n");

END