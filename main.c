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

#define DEFAULT_N 80
#define DEFAULT_M 80
#define DEFAULT_H 0.1
#define DEFAULT_DT 0.001

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
#define MIN 0.0
#define MAX 1.0
#define WARM_SIZE 1
#define WARM_FACTOR 2
#define MAX_ITER 200

double h, dt, pow_h;
int N, M, zeroCounter;
double** tab;
//double** prevTab;

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
    //    prevTab = calloc(M, sizeof (double*));

    int n;
    for (n = 0; n < N; n++) {
        tab[n] = calloc(M, sizeof (double));
    }
    int m;
    int ctr=1;
    for (n = 0; n < N; n++) {
        for (m = 0; m < M; m++) {
            tab[n][m] = initializeValue(n, m, N, M);
//            tab[n][m]=ctr++;
        }
    }

    return tab;
}

//double*** initializeResultTable(double*** tab) {
//
//    tab = calloc(M, sizeof (double**));
//    //    prevTab = calloc(M, sizeof (double*));
//
//    int i;
//    for (i = 0; i < N; i++) {
//        tab[i] = calloc(N, sizeof (double*));
//    }
//
//    int j;
//    for (i = 0; i < N; i++) {
//        for (j = 0; j < MAX_ITER; j++) {
//            tab[i][j] = calloc(MAX_ITER, sizeof (double));
//        }
//    }
//    return tab;
//}

void printfTable(double** tab) {

    printf("tab:\n");
    int n, m;
    for (n = 0; n < N; n++) {
        for (m = 0; m < M; m++) {
            printf("%1.3f |", tab[n][m]);
        }
        printf("\n");
    }
    printf("\n");
}

double drand(double low, double high) {
    return ( (double) rand() * (high - low)) / (double) RAND_MAX + low;
}

double calculateValue(double left, double right, double top, double bottom, double previous) {
//    double a = right + left + top + bottom;
//    printf("* %1.1f\n", a);
//    printf("* prev %1.1f\n", previous);
//    printf("dt %f\n", dt);
//    printf("h2 %f\n", pow_h);
    double value = (dt * (((right + left + top + bottom) - (4 * previous)) / pow_h)) + previous;
    if (value > 0) {
        return value;
    } else {
        zeroCounter++;
        return 0;
    }
}

void run() {
    int i, n, m;
    //    double*** T = initializeResultTable(T);

    double** prevTab = initializeTable(prevTab);
//    double T;

    for (i = 0; i < MAX_ITER; i++) {
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
//                printf("tab %1.1f\n", tab[n][m]);
//                printf("prevTab %1.1f\n", prevTab[n][m]);
//                printf("left %1.1f\n", left);
//                printf("right %1.1f\n", right);
//                printf("top %1.1f\n", top);
//                printf("bottom %1.1f\n", bottom);
                tab[n][m] = calculateValue(left, right, top, bottom, prevTab[n][m]);
//                printf("#tab %1.1f\n", tab[n][m]);

                prevTab[n][m] = tab[n][m];
            }

        }
//        break;
        printf("================== i=%d ===================\n", i);
//        printfTable(tab);
        if (zeroCounter == N * M) {
            break;
        }
    }
    printfTable(tab);


}

/*
 * Usage:
 *  ./pcam 100 100 0.001 0.01
 * 
 * Arguments initializes variables like below:
 * m = 100
 * n = 100
 * h = 0.001
 * dt = 0.01
 * 
 */
int main(int argc, char** argv) {

    if (argc < 5) {
        printf("Initialized default values. \n");
        N = DEFAULT_N;
        M = DEFAULT_M;
        h = DEFAULT_H;
        dt = DEFAULT_DT;
    } else {
        N = atof(argv[2]);
        M = atof(argv[1]);
        h = atof(argv[3]);
        dt = atof(argv[4]);
    }
    pow_h = h*h;
    //    printf("a %d \n", M);
    tab = initializeTable(tab);
    //    tab = initializeTable(prevTab);
    //    int a[5][2] = { {0,0}, {1,2}, {2,4}, {3,6},{4,8}};
    printfTable(tab);

    run(tab);

    //    printf("previous tab:\n");
    //    printfTable(prevTab);
    //    double r = drand48();


    //    printf("%f",drand(0.0, 1.0));
    return (EXIT_SUCCESS);
}



