/* 
 * File:   main.c
 * Author: piotrek
 *
 * PCAM implementation
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_N 10000
#define DEFAULT_M 10000
#define DEFAULT_H 0.1
#define DEFAULT_DT 0.001
#define DEFAULT_MAX_ITER 100
#define AVG_ITER 10

#define MIN 0.0
#define MAX 1.0
#define WARM_SIZE 1

double h, dt, pow_h;
int N, M, zeroCounter, maxIter;
double elapsed;
clock_t start, end;

double initializeValue(int row, int col, int N, int M) {
    if (col == 0 || (row == N - 1 || row == 0)) {
        return 0.0;
    } else if (col == M - 1 || (row == N - 1 || row == 0)) {
        return 0.0;
    } else if (row == N - 1 || row == 0) {
        return 0.0;
    } else {
        return 1.0;
    }
}

double** initializeTable(double** tab) {

    tab = calloc(N, sizeof (double*));

    int row;
    for (row = 0; row < N; row++) {
        tab[row] = calloc(M, sizeof (double));
    }
    int m;
    for (row = 0; row < N; row++) {
        for (m = 0; m < M; m++) {
            tab[row][m] = initializeValue(row, m, N, M);
        }
    }
    return tab;
}

void printfTable(double** tab) {

    printf("\ntab:\n");
    int row, m;
    for (row = 0; row < N; row++) {
        for (m = 0; m < M; m++) {
            printf("%1.3f |", tab[row][m]);
        }
        printf("\n");
    }
    printf("\n");
}

double calculateValue(double left, double right, double top, double bottom, double previous) {
    double value = (dt * (((right + left + top + bottom) - (4 * previous)) / pow_h)) + previous;
    if (value > 0) {
        return value;
    } else {
        zeroCounter++;
        return 0;
    }
}

void saveTable(double **table, int i, int clear) {
    char fileName[40];
    FILE *file;
    sprintf(fileName, "result.txt");

    if (clear == 1) {
        file = fopen(fileName, "wb");
    } else {
        file = fopen(fileName, "a");
    }
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, "\n====================================== %d ======================================\n", i);
    int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < M; col++) {

            fprintf(file, "%1.3f | ", table[col][row]);
        }
        fprintf(file, "\n\n");
    }
    fclose(file);
}

void run() {

    int j;
    for (j = 0; j < AVG_ITER; j++) {
        start = clock();

        int i, n, m;

        double** prevTab = initializeTable(prevTab);

        double** tab = initializeTable(tab);

        for (i = 0; i < maxIter; i++) {
            for (n = 0; n < N; n++) {
                for (m = 0; m < M; m++) {
                    double left = 0, right = 0, top = 0, bottom = 0;
                    if (n > 0) {
                        top = prevTab[n - 1][m];
                    }
                    if (n < N - 1) {
                        bottom = prevTab[n + 1][m];
                    }
                    if (m < M - 1) {
                        right = prevTab[n][m + 1];
                    }
                    if (m > 0) {
                        left = prevTab[n][m - 1];
                    }
                    tab[n][m] = calculateValue(left, right, top, bottom, prevTab[n][m]);
                }
            }
            //        printf("i=%d\n", i);
            if (zeroCounter == N * M) {
                break;
            }
            *prevTab = *tab;
            printf("%d\n", i);
        }

        end = clock();
        elapsed = elapsed + ((double) (end - start) / CLOCKS_PER_SEC) / AVG_ITER;
        
    }
    printf("\n%1.3f\t%d\n", elapsed, N);
    //    printfTable(tab);

    //    saveTable(tab, 0, 1);
}

/*
 * Usage:
 *  ./pcam 100 100 0.001 0.01 500
 * 
 * Arguments initializes variables like below:
 * n = 100 - rows
 * m = 100 - cols
 * h = 0.001
 * dt = 0.01
 * maxIter = 
 * 
 */
int main(int argc, char** argv) {

    if (argc >= 5) {
        N = atoi(argv[1]);
        M = atoi(argv[2]);
        h = atof(argv[3]);
        dt = atof(argv[4]);
        maxIter = DEFAULT_MAX_ITER;
    } else {
        printf("Initialized default values. \n");
        N = DEFAULT_N;
        M = DEFAULT_M;
        h = DEFAULT_H;
        dt = DEFAULT_DT;
        maxIter = DEFAULT_MAX_ITER;
    }
    if (argc == 6) {
        maxIter = atoi(argv[5]);
    }
    pow_h = h*h;




    //    printfTable(tab);

    run();





    return (EXIT_SUCCESS);
}



