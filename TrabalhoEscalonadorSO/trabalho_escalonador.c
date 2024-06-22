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

typedef struct Processor {
    char processes[MAX_PROCESSES];
    Tuple time;
    int startTime;
    int currentTime;
    int endTime;
} Processor;

void processTask(Processor *p, char processNames[][BUFFER_SIZE], int processTimes[], int lines);
void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]);
char *concatenateResults(char *results, char *processorResults);
Tuple findShortestIndex(int processTimes[], int lines);
int saveResults(char *results);

int main(int argc, char *argv[]){
    char *filename =  argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Erro ao encontrar o arquivo %s", filename);
        return 1;
    }

    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 5){
        printf("O segundo argumento deve ser entre 1 e 5");
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

    char processNames[lines][BUFFER_SIZE];
    int processTimes[lines];

    int i = 0;
    int j = 0;

    while(fgets(buffer, BUFFER_SIZE, fp)){
        last_token = strtok(buffer, delimiter_chars);
        while(last_token != NULL){
            if (i % 2 == 0){
                strcpy(processNames[j], last_token);   
            } else {
                processTimes[j] = atoi(last_token);
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


void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]) {
    Processor p1 = {"Processador_1\n", {lines + 1, INT_MAX}, 0, 0, 0};
    Processor p2 = {"Processador_2\n", {lines + 1, INT_MAX}, 0, 0, 0};
    Processor p3 = {"Processador_3\n", {lines + 1, INT_MAX}, 0, 0, 0};
    Processor p4 = {"Processador_4\n", {lines + 1, INT_MAX}, 0, 0, 0};
    Processor p5 = {"Processador_5\n", {lines + 1, INT_MAX}, 0, 0, 0};

    int numProcessors = atoi(argv[2]);
    Processor *processors[] = {&p1, &p2, &p3, &p4, &p5};

    for (int i = 0; i < lines; i += numProcessors) 
        for (int j = 0; j < numProcessors; ++j) 
            if (i + j < lines)
                processTask(processors[j], processNames, processTimes, lines);

    char *results = (char *)malloc(1);

    results[0] = '\0';

    for (int i = 0; i < numProcessors; ++i) 
        results = concatenateResults(results, processors[i]->processes);

    saveResults(results);
    free(results);
}

char *concatenateResults(char *results, char *processorResults){
    char *newResults = (char *)malloc(strlen(results) + strlen(processorResults) + 1);
    strcpy(newResults, results);
    strcat(newResults, processorResults);
    strcat(newResults, "\n");

    return newResults;
}

void processTask(Processor *p, char processNames[][BUFFER_SIZE], int processTimes[], int lines) {
    char info[BUFFER_SIZE];

    p->time = findShortestIndex(processTimes, lines);
    p->currentTime = processTimes[p->time.shortestIndex];
    processTimes[p->time.shortestIndex] = INT_MAX;

    p->endTime = p->currentTime + p->startTime;
    sprintf(info, "%s;%d;%d\n", processNames[p->time.shortestIndex], p->startTime, p->endTime);
    strcat(p->processes, info);

    p->startTime += p->currentTime;
}

Tuple findShortestIndex(int processTimes[], int lines){
    Tuple time = {lines + 1, INT_MAX};

    for(int i = 0; i < lines; ++i){
        if (processTimes[i] < time.shortestTime){
            time.shortestIndex = i;
            time.shortestTime = processTimes[i];
        }
    }

    return time;
}

int saveResults(char *results){
    FILE *fp = fopen("resultados.txt", "w");

    if (fp == NULL){
        printf("Erro ao criar o arquivo resultados.txt");
        return 1;
    }

    fprintf(fp, "%s", results);
    fclose(fp);

    return 0;
}