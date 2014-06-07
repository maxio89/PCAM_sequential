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

#define DEFAULT_N 10
#define DEFAULT_M 20
#define DEFAULT_H 0.1
#define DEFAULT_DT 0.001
#define DEFAULT_MAX_ITER 100

#define MIN 0.0
#define MAX 1.0
#define WARM_SIZE 1

double h, dt, pow_h;
int N, M, zeroCounter, maxIter;
double** tab, elapsed;
clock_t start, end;

double initializeValue(int i, int j, int N, int M) {

    int borderN = ((N * WARM_SIZE) / 4) - 1;
    int borderM = ((M * WARM_SIZE) / 4) - 1;
    if (i >= borderN && i < (N - borderN) && j >= borderM && j < (M - borderM)) {
        return 1.0;
    } else {
        return 0.0;
    }
}

double** initializeTable(double** tab) {

    tab = calloc(N, sizeof (double*));

    int n;
    for (n = 0; n < N; n++) {
        tab[n] = calloc(M, sizeof (double));
    }
    int m;
    for (n = 0; n < N; n++) {
        for (m = 0; m < M; m++) {
            tab[n][m] = initializeValue(n, m, N, M);
        }
    }

    return tab;
}

void printfTable(double** tab) {

    printf("\ntab:\n");
    int n, m;
    for (n = 0; n < N; n++) {
        for (m = 0; m < M; m++) {
            printf("%1.3f |", tab[n][m]);
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

void run(double** tab) {
    int i, n, m;

    double** prevTab = initializeTable(prevTab);

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
                prevTab[n][m] = tab[n][m];
            }

        }
        printf("i=%d\n", i);
        if (zeroCounter == N * M) {
            break;
        }
    }
    printfTable(tab);


}

/*
 * Usage:
 *  ./pcam 100 100 0.001 0.01 500
 * 
 * Arguments initializes variables like below:
 * m = 100
 * n = 100
 * h = 0.001
 * dt = 0.01
 * maxIter = 
 * 
 */
int main(int argc, char** argv) {

    if (argc >= 5) {
        N = atof(argv[2]);
        M = atof(argv[1]);
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
        maxIter = atof(argv[5]);
    }
    pow_h = h*h;

    start = clock();

    tab = initializeTable(tab);
    printfTable(tab);

    run(tab);

    end = clock();
    elapsed = (double) (end - start) / CLOCKS_PER_SEC;

    printf("\nTook %1.3f seconds\n", elapsed);

    return (EXIT_SUCCESS);
}



