#include "backend.h"

void scale_array(data_t *data, long double current_scale,
                 long double previous_scale) {
  for (int i = 0; data->array && i < (data->count_of_verticles + 1) * 3; i++) {
    data->array[i] *= current_scale / previous_scale;
  }
}

void move_by_axis(data_t *data, long double current_position,
                  long double previous_position, int axis) {
  for (int i = axis; data->array && i < (data->count_of_verticles + 1) * 3;
       i += 3) {
    data->array[i] += (current_position - previous_position);
  }
}

void move_x_axis(data_t *data, long double current_position,
                 long double previous_position) {
  move_by_axis(data, current_position, previous_position, X);
}

void move_y_axis(data_t *data, long double current_position,
                 long double previous_position) {
  move_by_axis(data, current_position, previous_position, Y);
}

void move_z_axis(data_t *data, long double current_position,
                 long double previous_position) {
  move_by_axis(data, current_position, previous_position, Z);
}

void rotate_by_axis(data_t *data, long double current_angle,
                    long double previous_angle, int axis) {
  current_angle = degrees_to_radians(current_angle);
  previous_angle = degrees_to_radians(previous_angle);

  for (int i = 0; data->array && i < (data->count_of_verticles + 1) * 3;
       i += 3) {
    long double x_derivative = data->array[i];
    long double y_derivative = data->array[i + 1];
    long double z_derivative = data->array[i + 2];

    if (axis == X) {
      data->array[i + 1] =
          y_derivative * cosl(current_angle - previous_angle) +
          z_derivative * sinl(current_angle - previous_angle);  // y'
      data->array[i + 2] =
          -y_derivative * sinl(current_angle - previous_angle) +
          z_derivative * cosl(current_angle - previous_angle);  // z'
    } else if (axis == Y) {
      data->array[i] =
          x_derivative * cosl(current_angle - previous_angle) +
          z_derivative * sinl(current_angle - previous_angle);  // x'
      data->array[i + 2] =
          -x_derivative * sinl(current_angle - previous_angle) +
          z_derivative * cosl(current_angle - previous_angle);  // z'
    } else if (axis == Z) {
      data->array[i] =
          x_derivative * cosl(current_angle - previous_angle) -
          y_derivative * sinl(current_angle - previous_angle);  // x'
      data->array[i + 1] =
          x_derivative * sinl(current_angle - previous_angle) +
          y_derivative * cosl(current_angle - previous_angle);  // y'
    }
  }
}

void rotate_x_axis(data_t *data, long double current_angle,
                   long double previous_angle) {
  rotate_by_axis(data, current_angle, previous_angle, X);
}

void rotate_y_axis(data_t *data, long double current_angle,
                   long double previous_angle) {
  rotate_by_axis(data, current_angle, previous_angle, Y);
}

void rotate_z_axis(data_t *data, long double current_angle,
                   long double previous_angle) {
  rotate_by_axis(data, current_angle, previous_angle, Z);
}

long double degrees_to_radians(long double angle) {
  return angle * Math_PI / 180.0;
}

int normalize_angle(int value) {
  while (value > 180) {
    value -= 360;
  }
  while (value < -180) {
    value += 360;
  }
  return value;
}
