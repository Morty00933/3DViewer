#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

enum {
  OK = 0,
  CALCULATION_ERROR = 1,
  WRONG_MATRIX = 2,
};

int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
