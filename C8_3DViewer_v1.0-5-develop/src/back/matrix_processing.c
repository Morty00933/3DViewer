#include "matrix_processing.h"

int create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0) {
    return WRONG_MATRIX;
  }

  result->matrix = (double **)calloc(
      1, rows * sizeof(double *) + columns * rows * sizeof(double));
  if (result->matrix == NULL) {
    return CALCULATION_ERROR;
  }

  result->matrix[0] = (double *)(result->matrix + rows);
  for (int i = 1; i < rows; i++) {
    result->matrix[i] = result->matrix[0] + i * columns;
  }

  result->rows = rows;
  result->columns = columns;
  return OK;
}

void remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    free(A->matrix);
    A->matrix = NULL;
  }
  A->rows = 0;
  A->columns = 0;
}
