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

#define DEFAULT_ROW 10
#define DEFAULT_COL 10
#define DEFAULT_H 0.1
#define DEFAULT_DT 0.001
#define DEFAULT_MAX_ITER 500
#define AVG_ITER 1

#define MIN 0.0
#define MAX 1.0
#define WARM_SIZE 1

double h, dt, pow_h;
int ROW, COL, zeroCounter, maxIter;
double elapsed;
clock_t start, end;

double initializeValue(int row, int col, int ROW, int COL) {
    if (col == 0 || (row == ROW - 1 || row == 0)) {
        return 0.0;
    } else if (col == COL - 1 || (row == ROW - 1 || row == 0)) {
        return 0.0;
    } else if (row == ROW - 1 || row == 0) {
        return 0.0;
    } else {
        return 1.0;
    }
}

double** initializeTable(double** tab) {

    tab = calloc(ROW, sizeof (double*));

    int row;
    for (row = 0; row < ROW; row++) {
        tab[row] = calloc(COL, sizeof (double));
    }
    int col;
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            tab[row][col] = initializeValue(row, col, ROW, COL);
        }
    }
    return tab;
}

void printfTable(double** tab) {

    printf("\ntab:\n");
    int row, col;
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {
            printf("%1.3f |", tab[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

double calculateValue(double left, double right, double top, double bottom, double previous) {
    double value = (dt * (((right + left + top + bottom) - (4 * previous)) / pow_h)) + previous;
    if (value > 0.0) {
        return value;
    } else {
        zeroCounter++;
        return 0.0;
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
    for (row = 0; row < ROW; row++) {
        for (col = 0; col < COL; col++) {

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

        int i, row, col;

        double** prevTab = initializeTable(prevTab), **prevPtr;

        double** tab = initializeTable(tab);

        for (i = 0; i < maxIter; i++) {
            for (row = 0; row < ROW; row++) {
                for (col = 0; col < COL; col++) {
                    double left = 0, right = 0, top = 0, bottom = 0;
                    if (row > 0) {
                        top = prevTab[row - 1][col];
                    }
                    if (row < ROW - 1) {
                        bottom = prevTab[row + 1][col];
                    }
                    if (col < COL - 1) {
                        right = prevTab[row][col + 1];
                    }
                    if (col > 0) {
                        left = prevTab[row][col - 1];
                    }
                    tab[row][col] = calculateValue(left, right, top, bottom, prevTab[row][col]);
                }
            }
            if (zeroCounter == ROW * COL) {
                break;
            }
//            printfTable(tab);
            prevPtr = prevTab;
            prevTab = tab;
            tab = prevPtr;
            printf("%d\n", i);
//            break;
        }

        end = clock();
        elapsed = elapsed + ((double) (end - start) / CLOCKS_PER_SEC);
        printfTable(tab);
    }
    printf("\n%1.3f\t%d\n", elapsed / AVG_ITER, ROW);
        

    //    saveTable(tab, 0, 1);
}

/*
 * Usage:
 *  ./pcam 100 100 0.001 0.01 500
 * 
 * Arguments initializes variables like below:
 * row = 100 - rows
 * col = 100 - cols
 * h = 0.001
 * dt = 0.01
 * maxIter = 
 * 
 */
int main(int argc, char** argv) {

    if (argc >= 5) {
        ROW = atoi(argv[1]);
        COL = atoi(argv[2]);
        h = atof(argv[3]);
        dt = atof(argv[4]);
        maxIter = DEFAULT_MAX_ITER;
    } else {
        printf("Initialized default values. \n");
        ROW = DEFAULT_ROW;
        COL = DEFAULT_COL;
        h = DEFAULT_H;
        dt = DEFAULT_DT;
        maxIter = DEFAULT_MAX_ITER;
    }
    if (argc == 6) {
        maxIter = atoi(argv[5]);
    }
    pow_h = h*h;

    run();

    return (EXIT_SUCCESS);
}



