#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_processing.h"

#define Math_PI 3.14159265358979323846264338327950288
#define S_SIZE 1024
#define X 0
#define Y 1
#define Z 2

typedef struct facets {
  unsigned int *verticles;
  int verticles_in_facets;
} polygon_t;

typedef struct data {
  int count_of_verticles;
  int count_of_facets;
  double *array;
  double max_coord;
  matrix_t matrix_3d;
  polygon_t *polygons;
  unsigned int *array_polygons;
  unsigned int size_polygons;
  double *array_const;
} data_t;

// parsing

int file_parsing(data_t *data, char *object_file_name);

int parsing_data_size(data_t *data, char *object_file_name);
int line_parsing(char c, char *string_file, int *step);

void remove_data(data_t *data);
void remove_array(data_t *data);
void remove_polygons(data_t *data);

int search_polygons(polygon_t *polygons, char *string_file);
int string_to_double(char *str, int *step, double *number);
int search_exponent(char *str, int *step, double *num);

int create_polygons_array(data_t *data);
int matrix_to_array(matrix_t matrix, double *array_1, double *array_2);

void search_max_coord(data_t data, double *max_coord);
int skip_spaces(char *str, int *step);
int count_digits(int num);

// transformations

void scale_array(data_t *data, long double current_scale,
                 long double previous_scale);

void move_by_axis(data_t *data, long double current_position,
                  long double previous_position, int axis);
void move_x_axis(data_t *data, long double current_position,
                 long double previous_position);
void move_y_axis(data_t *data, long double current_position,
                 long double previous_position);
void move_z_axis(data_t *data, long double current_position,
                 long double previous_position);

void rotate_by_axis(data_t *data, long double current_angle,
                    long double previous_angle, int axis);
void rotate_x_axis(data_t *data, long double current_angle,
                   long double previous_angle);
void rotate_y_axis(data_t *data, long double current_angle,
                   long double previous_angle);
void rotate_z_axis(data_t *data, long double current_angle,
                   long double previous_angle);

long double degrees_to_radians(long double angle);
int normalize_angle(int value);
