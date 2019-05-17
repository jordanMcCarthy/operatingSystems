#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int wordCount(FILE *);



int main(int argc, char *argv[]){
    
    int numberOfFiles = argc - 1; // number of files provided
    int i; // counter variable used for looping through files 
    int theCount; // word count for file
    
    // creates seperate processes for each file provided
    // the file is then opened and sent to the wordCount
    // function which returns the amount of words in file
    for (i = 1; i <= numberOfFiles; i++){
        if (fork() == 0){
            FILE *fp = fopen(argv[i], "r");
            if(!fp){
                perror("Unable to open file!\n");
                exit(EXIT_FAILURE);
            }
            theCount = wordCount(fp);
            printf("Child process for %s: number of words is %d\n", argv[i], theCount);
            exit(0);
        }
    }
    for (i = 1; i <= numberOfFiles; i++){
            wait(NULL);
    }    

    printf("All %d files have been counted!\n", numberOfFiles);
}


// wordCount takes in a file, runs word count algorithm on said file
// and returns the word count
int wordCount(FILE *in){
    
    char ch; // variable to store and analyze character in file
    char inWord = 0; // flag to se
    int wordCount = 0; // counter variable for sum of words provided

    // while loop to read until EOF
    while(!feof(in)){
        ch = (char)getc(in); // reads next character from file and stores it for analysis
        
        if(ferror(in)){
            perror("Read error!\n");
            exit(EXIT_FAILURE);
        }
        
        // word count algorithm 
        // first checks for EOF then if character is not whitespace and if the inWord flag has not been set
        // if so the inWord flag is set and the word count is incremented
        if(!feof(in)){
            if(!iswspace(ch)){
                if (!inWord){
                    inWord = 1; 
                    wordCount++;
                }
            }else{ 
                inWord = 0;
            }
       }
    }
    return wordCount;
}
