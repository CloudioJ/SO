#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_PROCESSES 10000
#define TRUE 1
#define FALSE 0

typedef struct Tuple {
    int shortestIndex;
    int shortestTime;
} Tuple;

void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]);
Tuple findShortestIndex(int processTimes[], int lines);

int main(int argc, char *argv[]){
    char *filename =  argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Erro ao encontrar o arquivo %s", filename);
        return 1;
    }

    if (*argv[2] != '1' && *argv[2] != '2'){
        printf("O segundo argumento deve ser 1 ou 2");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char *delimiter_chars = " ";
    char *last_token;
    int lines = 1;

    char ch;

    while((ch = getc(fp)) != EOF){
        if (ch == '\n'){
            ++lines;
        }
    }

    rewind(fp);

    // printf("lines: %d\n", lines);
    char processNames[lines][BUFFER_SIZE];
    int processTimes[lines];

    int i = 0;
    int j = 0;

    while(fgets(buffer, BUFFER_SIZE, fp)){
        last_token = strtok(buffer, delimiter_chars);
        while(last_token != NULL){
            if (i % 2 == 0){
                strcpy(processNames[j], last_token);
                // printf("%s\n", processNames[j]);    
            } else {
                processTimes[j] = atoi(last_token);
                // printf("%d\n", processTimes[j]);
                ++j;
            }
            last_token = strtok(NULL, delimiter_chars);
            ++i;
        }

    }

    fclose(fp);
    sjf(processNames, processTimes, lines, argv);

    return 0;
}

void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]){
    
    char processor1[MAX_PROCESSES] = "Processador_1\n";
    char processor2[MAX_PROCESSES] = "Processador_2\n";
    char info[BUFFER_SIZE];

    switch (*argv[2]){
        case '1':
            int startTime = 0;
            int currentTime = 0;
            Tuple t = {lines + 1, INT_MAX};

            for(int i = 0; i < lines; ++i){
                t = findShortestIndex(processTimes, lines);

                currentTime = processTimes[t.shortestIndex];
                processTimes[t.shortestIndex] = INT_MAX;

                sprintf(info, "%s;%d;%d\n", processNames[t.shortestIndex], startTime, currentTime + startTime);
                strcat(processor1, info);

                startTime += currentTime;
            }
            break;

        case '2':
            Tuple t1 = {lines + 1, INT_MAX};
            Tuple t2 = {lines + 1, INT_MAX};
            int startTime1 = 0;
            int startTime2 = 0;
            int done[lines];

            for(int i = 0; i < lines; i += 2){
                t1 = findShortestIndex(processTimes, lines);

                int currentTime1 = processTimes[t1.shortestIndex];
                processTimes[t1.shortestIndex] = INT_MAX;
                
                for(int j = 0; j < lines; ++j){
                    if (j != t1.shortestIndex){
                        t2 = findShortestIndex(processTimes, lines);
                        break;
                    }
                }

                int currentTime2 = processTimes[t2.shortestIndex];
                processTimes[t2.shortestIndex] = INT_MAX;

                sprintf(info, "%s;%d;%d\n", processNames[t1.shortestIndex], startTime1, currentTime1 + startTime1);
                strcat(processor1, info);

                sprintf(info, "%s;%d;%d\n", processNames[t2.shortestIndex], startTime2, currentTime2 + startTime2);
                strcat(processor2, info);

                startTime1 += currentTime1;
                startTime2 += currentTime2;
            }
        break;
    }

    printf("GETTING HERE\n");
    printf("%s\n", processor1);
    printf("%s", processor2);
}

Tuple findShortestIndex(int processTimes[], int lines){
    Tuple time = {lines + 1, INT_MAX};

    for(int i = 0; i < lines; ++i){
        if (processTimes[i] <= time.shortestTime){
            time.shortestIndex = i;
            time.shortestTime = processTimes[i];
        }
    }

    return time;
}

int doneCheck(int done[], int lines){
    for(int i = 0; i < lines; ++i){
        if (done[i] == FALSE){
            return FALSE;
        }
    }

    return TRUE;
}