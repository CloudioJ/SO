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
    int index;
    int time;
} Tuple;

typedef struct Processor {
    char processes[MAX_PROCESSES];
    Tuple time;
    int startTime;
    int currentTime;
    int endTime;
    int busy;
} Processor;

int processTask(Processor *p, char processNames[][BUFFER_SIZE], int processTimes[], int lines, int useLJF, int numProcessors);
void processTaskLJF(Processor *p, char processNames[][BUFFER_SIZE], int processTimes[], int lines);
void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]);
void ljf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]);
void updateProcessors(Processor *processors[], int numProcessors, int currentEndtime, int currentIndex);
char *concatenateResults(char *results, char *processorResults);
Tuple findShortestIndex(int processTimes[], int lines);
Tuple findLongestIndex(int processTimes[], int lines);
int *intdup(int const *src, size_t len);
int saveResults(char *results, char *filename);

int full = TRUE;

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Erro ao encontrar o arquivo %s", filename);
        return 1;
    }

    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 5) {
        printf("O segundo argumento deve ser entre 1 e 5");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char *delimiter_chars = " ";
    char *last_token;
    int lines = 1;

    char ch;

    while ((ch = getc(fp)) != EOF) {
        if (ch == '\n') {
            ++lines;
        }
    }

    rewind(fp);

    char processNames[lines][BUFFER_SIZE];
    int processTimes[lines];

    int i = 0;
    int j = 0;

    while (fgets(buffer, BUFFER_SIZE, fp)) {
        last_token = strtok(buffer, delimiter_chars);
        while (last_token != NULL) {
            if (i % 2 == 0) {
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
    ljf(processNames, processTimes, lines, argv);

    return 0;
}

void sjf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]) {
    Processor p1 = {"Processador_1\n", {lines + 1, INT_MAX}, 0, 0, 0, FALSE};
    Processor p2 = {"Processador_2\n", {lines + 1, INT_MAX}, 0, 0, 0, FALSE};
    Processor p3 = {"Processador_3\n", {lines + 1, INT_MAX}, 0, 0, 0, FALSE};
    Processor p4 = {"Processador_4\n", {lines + 1, INT_MAX}, 0, 0, 0, FALSE};
    Processor p5 = {"Processador_5\n", {lines + 1, INT_MAX}, 0, 0, 0, FALSE};

    int *aux = intdup(processTimes, lines);

    int numProcessors = atoi(argv[2]);
    Processor *processors[] = {&p1, &p2, &p3, &p4, &p5};
    int currentEndTime = 0, i = 0, j = 0;

    for (i = 0; i < lines; i += numProcessors) {
        for (j = 0; j < numProcessors; ++j) {
            if ((!processors[j]->busy && i + j < lines) || (full && i + j < lines)) {
                currentEndTime = processTask(processors[j], processNames, aux, lines, FALSE, numProcessors);
            }
            updateProcessors(processors, numProcessors, currentEndTime, j);
        }
    }

    char *results = (char *)malloc(1);

    results[0] = '\0';

    for (int i = 0; i < numProcessors; ++i)
        results = concatenateResults(results, processors[i]->processes);

    saveResults(results, "resultados.txt");
    free(results);
}

void ljf(char processNames[][BUFFER_SIZE], int processTimes[], int lines, char *argv[]) {
    Processor p1 = {"Processador_1\n", {lines + 1, 0}, 0, 0, 0, FALSE};
    Processor p2 = {"Processador_2\n", {lines + 1, 0}, 0, 0, 0, FALSE};
    Processor p3 = {"Processador_3\n", {lines + 1, 0}, 0, 0, 0, FALSE};
    Processor p4 = {"Processador_4\n", {lines + 1, 0}, 0, 0, 0, FALSE};
    Processor p5 = {"Processador_5\n", {lines + 1, 0}, 0, 0, 0, FALSE};

    int numProcessors = atoi(argv[2]);
    int *aux = intdup(processTimes, lines);
    int currentEndTime = 0, i = 0, j = 0;

    Processor *processors[] = {&p1, &p2, &p3, &p4, &p5};

    for (i = 0; i < lines; i += numProcessors) {
        for (j = 0; j < numProcessors; ++j) {
            if ((!processors[j]->busy && i + j < lines) || (full && i + j < lines)) {
                currentEndTime = processTask(processors[j], processNames, aux, lines, TRUE, numProcessors);
            }
            updateProcessors(processors, numProcessors, currentEndTime, j);
        }
    }

    char *results = (char *)malloc(1);

    results[0] = '\0';

    for (int i = 0; i < numProcessors; ++i)
        results = concatenateResults(results, processors[i]->processes);

    saveResults(results, "resultados_ljf.txt");
    free(results);
}

char *concatenateResults(char *results, char *processorResults) {
    char *newResults = (char *)malloc(strlen(results) + strlen(processorResults) + 1);
    strcpy(newResults, results);
    strcat(newResults, processorResults);
    strcat(newResults, "\n");

    return newResults;
}

Tuple findLongestIndex(int processTimes[], int lines) {
    Tuple time = {lines + 1, 0};

    for (int i = 0; i < lines; ++i) {
        if (processTimes[i] > time.time) {
            time.index = i;
            time.time = processTimes[i];
        }
    }

    return time;
}

int processTask(Processor *p, char processNames[][BUFFER_SIZE], int processTimes[], int lines, int useLJF, int numProcessors) {
    char info[BUFFER_SIZE];

    if (useLJF) {
        p->time = findLongestIndex(processTimes, lines);
        p->currentTime = processTimes[p->time.index];
        processTimes[p->time.index] = 0;
    } else {
        p->time = findShortestIndex(processTimes, lines);
        p->currentTime = processTimes[p->time.index];
        processTimes[p->time.index] = INT_MAX;
    }

    p->endTime = p->currentTime + p->startTime;
    sprintf(info, "%s;%d;%d\n", processNames[p->time.index], p->startTime, p->endTime);
    strcat(p->processes, info);

    p->startTime += p->currentTime;
    p->busy = TRUE;

    return p->endTime;
}

Tuple findShortestIndex(int processTimes[], int lines) {
    Tuple time = {lines + 1, INT_MAX};

    for (int i = 0; i < lines; ++i) {
        if (processTimes[i] < time.time) {
            time.index = i;
            time.time = processTimes[i];
        }
    }

    return time;
}

void updateProcessors(Processor *processors[], int numProcessors, int currentEndtime, int currentIndex) {
    for (int i = 0; i < numProcessors; ++i) {
        if (processors[i]->busy && processors[i]->endTime <= currentEndtime && i != currentIndex) {
            processors[i]->busy = FALSE;
            full = FALSE;
        } else {
            full = TRUE;
        }
    }
}

int *intdup(int const *src, size_t len) {
    int *p = malloc(len * sizeof(int));
    memcpy(p, src, len * sizeof(int));
    return p;
}

int saveResults(char *results, char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Erro ao criar o arquivo de resultados");
        return 1;
    }

    fprintf(fp, "%s", results);
    fclose(fp);

    return 0;
}
