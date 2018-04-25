/* 
CS288 HOMEWORK 8
Your program will take in two command-line parameters: n and error
command: jacobi 5 0.0001
command: jacobi 10 0.000001
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <float.h>

#define N 100
#define MAX_ITER 10000
#define SPECIFIC 0 //Boolean to decide if we generate specific matrix or Prof's random one.

int  jacobi();
int  convergence();
int  checkValid();

void init();
void print_equation();
void print_Solution();
void srand();
void specificInit();

float a[N][N], b[N];
float x[N], buf[N];
float error;

int n, iter;

int main(int argc, char **argv) {
      error = atof(argv[2]);

      if (SPECIFIC) { 
            n = 3;
            specificInit();
            jacobi();
      } else {
            n = atoi(argv[1]);
            init();                 /* initalize a, x0 and b - DO not change */
            jacobi();
      }
      return 0;
}

int jacobi(){
      int i, j;
      float sum;

      iter = 0;

      while (!convergence() && iter < MAX_ITER) {
            for (i = 0; i < n; i++) {
                  sum = 0;
                  for (j = 0; j < n; j++) {
                        if (i != j) { sum = sum + (a[i][j] * x[j]); } 
                  }
                  buf[i] = ( b[i] - sum ) / a[i][i];
            }
            for (i = 0; i < n; i++) {
                  x[i] = buf[i];
            }
            iter++;
      }
      print_Solution();
}

// returns 1 if converged else 0
int convergence() {
      int i, j;
      float sum;

      for (i = 0; i < n; i++) {
            sum = 0;

            for (j = 0; j < n; j++) {
                  sum = sum + ( a[i][j] * x[j] );  
            }

            //We use fabs() instead of abs() because abs()
            //returns 0 on negative floats???
            if(fabs(sum - b[i]) > error) {
                  return 0;
            }
      }
      return 1;
}

// Try not to change this. Use it as is.
void init(char **argv) {
      int i, j, flag, seed;
      float sum;

      flag = 0;
      seed = time(0) % 100;
      
      srand(seed);

      for (i = 0; i < n; i++) {
            sum = 0;

            for (j = 0; j < n; j++) {
                  a[i][j] = rand() & 0x7;
                  if (rand() & 0x1) { a[i][j] = -a[i][j]; } 
            }

            for (j = 0; j < n; j++) {
                  if (i != j) { sum = sum + abs(a[i][j]); } 
            } 

            if (a[i][i] < sum) { a[i][i] = sum + a[i][i]; }
      }

      for (i = 0; i < n; i++) { x[i] = 1; }

      srand(seed);

      for (i = 0; i < n; i++) {
            b[i] = rand() & 0x7;
            if (rand() & 0x1) { b[i] = -b[i]; }
      }

      for (i = 0; i < n; i++) {
            x[i] = 0;
      }

      print_equation();
}

//If we want to specify a particular matrix, not a random one
void specificInit() {
      int i;
      //A[N][N]
      //First row
      a[0][0] = -3; a[0][1] = -1; a[0][2] = -1;
      //Second row
      a[1][0] = 2;  a[1][1] = 8;  a[1][2] = -3;
      //Third row
      a[2][0] = -4; a[2][1] = 3;  a[2][2] = 8;

      //B[N]
      b[0] = -5; b[1] = -1; b[2] = -1;

      //x[N]
      for (i = 0; i < n; i++) {
            x[i] = 0;
      }
      print_equation();
}

int checkValid() {
      int i;

      for (i = 0; i < n; i++) {
            if (x[i] != x[i]) { return 0; } //NaN solution
            if (x[i] > DBL_MAX || x[i] < -DBL_MAX) { return 0; } //-INF or +INF solution 
      }
      return 1;
}

void print_Solution() {
      int i;

      if (convergence() && checkValid()) {
            printf("It took %d iterations to reach convergence.\n", iter);
            for (i = 0; i < n; i++) { printf("x%d = %f\n", i, x[i]); }
      } else {
            printf("No solution found.\n");
      }
}

void print_equation() {
      int i, j;
      printf("A * x = b\n");

      for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) { printf("%2d ", (int) a[i][j]); }
            printf(" * x%d = %d\n", i, (int) b[i]);
      }

      printf("\n");
}
// end of file