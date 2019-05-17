#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct word {
    char *str;          
    int freq;               
    struct word *pNext;     
} Word;

typedef struct thread_info{
    pthread_t thread_id;
    int thread_num;
    char *argv_string;
} thread_info;

int getNextWord(FILE *fp, char *buf, int bufsize);
void addWord(char *pWord);
Word* createWordCounter(char *word);
void doWork(void *arg);

pthread_mutex_t lock;
Word *pStart = NULL;
int main(int argc, char *argv[]){
    
    struct thread_info *tinfo;
    void *res;
    int numberOfFiles = argc - 1; 
    int tnum;
    
    pthread_mutex_init(&lock, NULL); // mutex lock initialized for usage to ensure synchronization
    tinfo = calloc(numberOfFiles, sizeof(struct thread_info));
    
    /* threads created for each file provided */
    for(tnum = 0; tnum < numberOfFiles; tnum++){
        tinfo[tnum].thread_num = tnum + 1;
        tinfo[tnum].argv_string = argv[tnum + 1];
        pthread_create(&tinfo[tnum].thread_id, NULL, (void*) doWork, &tinfo[tnum]);
    }
    /* each thread is joined back together */
    for(tnum = 0; tnum < numberOfFiles; tnum++){
        pthread_join(tinfo[tnum].thread_id,&res); 
        free(res);
    }
    
    pthread_mutex_destroy(&lock); // mutex lock is destroyed as it is no longer needed
    free(tinfo);
         
    printf("All %d files have been counted!\n", numberOfFiles);
    
    /* linked list is sorted alphabetically */
    Word *pCounter = NULL;
    Word *j; 
    pCounter = pStart;
    char *temp = malloc(sizeof(char *));
    int tempFreq = pCounter->freq;
    for(pCounter;pCounter->pNext != NULL; pCounter=pCounter->pNext){
        for(j=pCounter->pNext; j!=NULL;j=j->pNext){
            if(strcmp(pCounter->str,j->str) > 0){
                strcpy(temp, pCounter->str);
                tempFreq = pCounter->freq;
                strcpy(pCounter->str, j->str);
                pCounter->freq = j->freq;
                strcpy(j->str,temp);
                j->freq = tempFreq;
            }
        }
    }
    
    /* Word frequency is printed to console */
    pCounter = pStart;
    while(pCounter != NULL){
             printf("%s appears %d times\n", pCounter->str,pCounter->freq);
             pCounter = pCounter->pNext;

        }
    }

/* doWork is the critical section, each thread is able to add its words */
/* and frequency to the global word struct                             */
void doWork(void *arg){
        pthread_mutex_lock(&lock); // critical section begins
        struct thread_info *tinfo = arg;
        int uniquecount = 0; 
        int totalCount = 0;
        FILE *fp = fopen(tinfo->argv_string, "r");
        if(!fp){
            perror("Unable to open file!\n");
            exit(EXIT_FAILURE);
            }
        char buf[1024];
        int size = 1024;
        Word *pCounter = NULL;
        while (getNextWord(fp, buf, size)) {
            addWord(buf);
            totalCount++;
        }
        printf("Thread %d: number of words in %s is %d\n", tinfo->thread_num, tinfo->argv_string, totalCount);
        pCounter = pStart;
        while(pCounter != NULL){
            uniquecount++;
            pCounter = pCounter->pNext;
        }
        pthread_mutex_unlock(&lock); //critical section ends
}


/* word and frequency is added to linked list  */
/* if word exists its frequency is incremented */
void addWord(char *word)
{
    Word *pCounter = NULL;
    Word *pLast = NULL;

    if(pStart == NULL){
        pStart = createWordCounter(word);
        return;
    }
    
    pCounter = pStart;
    while(pCounter != NULL){
        if(strcmp(word, pCounter->str) == 0){
            ++pCounter->freq;

            return;
        }
        pLast = pCounter;            
        pCounter = pCounter->pNext;  
        
    }
   
    pLast->pNext = createWordCounter(word);
}

/* space is made for word in linked list */
Word* createWordCounter(char *word)
{
    Word *pCounter = NULL;
    pCounter = (Word*)malloc(sizeof(Word));
    pCounter->str = (char*)malloc(strlen(word)+1);
    strcpy(pCounter->str, word);
    pCounter->freq = 1;
    pCounter->pNext = NULL;
    return pCounter;
}

/* word is created to be analyzed by addWord function */
int getNextWord(FILE *fp, char *buf, int bufsize) {
        char *p = buf;
        char ch;

        while(!isalpha(ch)){
            ch = fgetc(fp);
            if (ch == EOF)
                return 0;
        }
     
        while(!iswspace(ch)){
            if(p - buf < bufsize - 1){
                *p++ = tolower(ch);
                ch = fgetc(fp);
            }
        } 
        *p = '\0';
        return 1;
}


