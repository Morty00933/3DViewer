#include "backend.h"

int file_parsing(data_t *data, char *object_file_name) {
  const int success = 1;
  const int failure = 0;
  const int vertices_per_facet = 3;
  const int max_line_size = S_SIZE;

  char line[max_line_size];
  line[0] = '\0';
  // char line[max_line_size] = {'\0'};ß
  FILE *file = NULL;
  matrix_t matrix = {0};
  polygon_t *polygons = NULL;
  int polygons_counter = 0;

  int flag = success;

  if (!parsing_data_size(data, object_file_name)) {
    flag = failure;
  }

  polygons = (polygon_t *)calloc(data->count_of_facets, sizeof(polygon_t));
  if (!polygons) {
    flag = failure;
  }

  if (create_matrix(data->count_of_verticles + 1, vertices_per_facet,
                    &matrix) != 0) {
    free(polygons);
    flag = failure;
  }

  file = fopen(object_file_name, "r");
  if (!file) {
    free(polygons);
    flag = failure;
  }

  int row = 1;
  // possibly try to use an alternative function:
  while (fgets(line, max_line_size, file)) {
    line[max_line_size - 1] = '\0';  // maybe delete this later

    int step = 0;
    if (line_parsing('v', line, &step)) {
      for (int i = 0; i < vertices_per_facet; i++) {
        int s = 0;
        double num = 0;
        string_to_double(&line[step], &s, &num);
        matrix.matrix[row][i] = num;
        step += s;
      }
      row++;
    } else if (line_parsing('f', line, 0)) {
      //
      if (!search_polygons(&polygons[polygons_counter], line)) {
        fclose(file);
        remove_polygons(data);
        flag = failure;
        break;
      }
      polygons_counter++;
    }
    line[0] = '\0';
  }

  fclose(file);

  data->matrix_3d = matrix;
  data->polygons = polygons;

  if (!create_polygons_array(data)) {
    remove_polygons(data);
    flag = failure;
  }

  data->array = (double *)calloc(
      (data->count_of_verticles + 1) * vertices_per_facet, sizeof(double));
  data->array_const = (double *)calloc(
      (data->count_of_verticles + 1) * vertices_per_facet, sizeof(double));

  if (!data->array || !data->array_const) {
    remove_polygons(data);
    remove_array(data);
    flag = failure;
  }

  matrix_to_array(data->matrix_3d, data->array, data->array_const);
  search_max_coord(*data, &data->max_coord);

  return flag;
}

int parsing_data_size(data_t *data, char *object_file_name) {
  const int success = 1;
  const int failure = 0;
  FILE *f;
  char string_file[S_SIZE] = {'\0'};
  int flag = success;

  if ((f = fopen(object_file_name, "r")) == NULL) {
    flag = failure;
  }

  while (fgets(string_file, S_SIZE, f)) {
    if (line_parsing('v', string_file, NULL)) {
      data->count_of_verticles++;
    } else if (line_parsing('f', string_file, NULL)) {
      data->count_of_facets++;
    }
  }

  fclose(f);
  return flag;
}

int line_parsing(char target, char *string_file, int *step) {
  const int condition_met = 1;
  const int condition_not_met = 0;
  int current_pos = 0;
  skip_spaces(string_file, &current_pos);
  int flag = condition_not_met;
  if (string_file[current_pos] == target &&
      string_file[current_pos + 1] == ' ') {
    if (step) {
      *step = current_pos + 2;
    }
    flag = condition_met;
  }

  if (step && flag == condition_not_met) {
    *step = current_pos;
  }

  return flag;
}

void remove_data(data_t *data) {
  remove_polygons(data);
  remove_matrix(&data->matrix_3d);
  remove_array(data);

  data->count_of_facets = 0;
  data->count_of_verticles = 0;
  data->size_polygons = 0;
  data->max_coord = 0;
  data->matrix_3d.matrix = NULL;
}

void remove_array(data_t *data) {
  if (data->array != NULL) {
    free(data->array);
    data->array = NULL;
  }
  if (data->array_const != NULL) {
    free(data->array_const);
    data->array_const = NULL;
  }
}

void remove_polygons(data_t *data) {
  if (data->polygons != NULL) {
    for (int i = 0; i < data->count_of_facets; i++) {
      polygon_t *polygon = &data->polygons[i];
      if (polygon->verticles != NULL) {
        free(polygon->verticles);
        polygon->verticles = NULL;
      }
    }
    free(data->polygons);
    data->polygons = NULL;
  }

  if (data->array_polygons != NULL) {
    free(data->array_polygons);
    data->array_polygons = NULL;
  }
}

int search_polygons(polygon_t *polygons, char *string_file) {
  int i = 0, step = 0;
  int flag = 1;
  polygons->verticles = (unsigned int *)calloc(128, sizeof(unsigned int));
  polygons->verticles_in_facets = 0;

  if (polygons->verticles == NULL) {
    flag = 0;
  } else {
    while (step < (int)strlen(string_file)) {
      int s = 0;
      double num = 0;
      if (string_to_double(&string_file[step], &s, &num)) {
        step += s;
        if (string_file[step - count_digits((int)num) - 1] == ' ') {
          if (polygons->verticles_in_facets != 0) {
            polygons->verticles[i++] = (unsigned int)num;
          }
          polygons->verticles[i++] = (unsigned int)num;
          polygons->verticles_in_facets++;
        }
      }
      step++;
    }

    if (polygons->verticles_in_facets > 1) {
      polygons->verticles[i++] = polygons->verticles[0];
    }
  }

  return flag;
}

int string_to_double(char *str, int *step, double *number) {
  int dot = 0, i = 0;
  int flag = 1, neg = 1;
  double num = 0;

  while (isspace((unsigned char)str[i])) {
    i++;
  }

  if (str[i] == '-' || str[i] == '+') {
    neg = (str[i] == '-') ? -1 : 1;
    i++;
  } else if (!isdigit((unsigned char)str[i])) {
    flag = 0;
  }

  if (flag == 1) {
    while (isdigit((unsigned char)str[i]) || str[i] == '.') {
      if (str[i] == '.') {
        dot = 10;
      } else {
        if (dot == 0) {
          num = (num * 10) + (double)(str[i] - '0');
        } else {
          num += (double)(str[i] - '0') / dot;
          dot *= 10;
        }
      }
      i++;
    }
  }

  *step = i;
  if (search_exponent(&str[i], &i, &num)) {
    *step = i;
  }

  *number = num * neg;
  return flag;
}

int search_exponent(char *str, int *step, double *num) {
  int e = 0, i = 0;
  int flag = 1, negative = 1;

  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    if (str[i] && str[i] == '-') {
      negative = -1;
      i++;
    } else if (str[i] && str[i] == '+') {
      negative = 1;
      i++;
    }
    if (str[i] && isdigit(str[i]) == 1) {
      e = (e * 10) + (int)(str[i] - '0');
      i++;
    } else {
      flag = 0;
    }
    if (str[i] && isdigit(str[i]) == 1) {
      e = (e * 10) + (int)(str[i] - '0');
    }
  } else {
    flag = 0;
  }
  double ten = 10;
  if (flag == 1) {
    if (negative == -1) {
      ten = 0.1;
    }
    for (int i = 0; i < e; i++) {
      *num *= ten;
    }
  }
  *step = i;

  return flag;
}

int create_polygons_array(data_t *data) {
  int flag = 1;
  for (int i = 0; i < data->count_of_facets; i++) {
    data->size_polygons += data->polygons[i].verticles_in_facets * 2;
  }
  data->array_polygons =
      (unsigned int *)calloc(data->size_polygons, sizeof(unsigned int));

  if (data->array_polygons != NULL) {
    data->size_polygons = 0;
    for (int i = 0; i < data->count_of_facets; i++) {
      for (int j = 0; j < data->polygons[i].verticles_in_facets * 2; j++) {
        data->array_polygons[data->size_polygons++] =
            data->polygons[i].verticles[j];
      }
    }
  } else {
    flag = 0;
  }
  return flag;
}

int matrix_to_array(matrix_t matrix, double *array_1, double *array_2) {
  int flag = 1;
  if (!array_1) {
    flag = 0;
  }

  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.columns; j++) {
      int array_index = i * matrix.columns + j;
      array_1[array_index] = array_2[array_index] = matrix.matrix[i][j];
    }
  }

  return flag;
}

void search_max_coord(data_t data, double *max_coord) {
  *max_coord = 0;

  for (int i = 0; i < data.count_of_verticles; i++) {
    *max_coord = fmax(*max_coord, fabs(data.array[i]));
  }

  *max_coord *= 1.2f;
}

int skip_spaces(char *str, int *step) {
  int flag = 0;
  *step = 0;
  while (isspace(str[*step]) == 1) {
    (*step)++;
    flag = 1;
  }
  return flag;
}

int count_digits(int num) {
  return (num == 0) ? 1 : (int)(log10(abs(num))) + 1;
}
