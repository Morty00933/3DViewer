#include <check.h>
#include <math.h>

#include "back/backend.h"

START_TEST(test_parsing_1) {
  data_t data = {0};
  char model_file_name[S_SIZE] = "./objects/test.obj";
  // create test_model.txt file with some test data
  int result = file_parsing(&data, model_file_name);
  // printf("%lf\n", data.matrix_3d.matrix[1][0]);
  ck_assert_int_eq(result, 1);
  // check that data.count_of_verticles is correct
  ck_assert_int_eq(data.count_of_verticles, 8);
  // check that data.count_of_facets is correct
  ck_assert_int_eq(data.count_of_facets, 12);
  // check that data.matrix_3d is correct
  // check that data.polygons is correct
  // check that data.array is correct
  // free memory
  remove_data(&data);
}
END_TEST

START_TEST(test_parsing_matrix) {
  data_t data = {0};
  char* model_file_name = "./objects/test.obj";
  int result = file_parsing(&data, model_file_name);
  ck_assert_int_eq(result, 1);
  ck_assert_int_eq(data.matrix_3d.rows, 9);
  ck_assert_int_eq(data.matrix_3d.columns, 3);
  ck_assert_double_eq(data.matrix_3d.matrix[1][0], 0.0);
  ck_assert_double_eq(data.matrix_3d.matrix[1][1], 0.0);
  ck_assert_double_eq(data.matrix_3d.matrix[1][2], 0.0);
  ck_assert_double_eq(data.matrix_3d.matrix[8][0], 1.0);
  ck_assert_double_eq(data.matrix_3d.matrix[8][1], 1.0);
  ck_assert_ldouble_eq_tol(data.matrix_3d.matrix[8][2], 1, 1);
  remove_data(&data);
}
END_TEST

END_TEST
START_TEST(test_parsing_3) {
  data_t data = {0};
  char model_file_name[] = "./objects/pyramid.obj";
  // create test_model.txt file with some test data
  int result = file_parsing(&data, model_file_name);
  ck_assert_int_eq(result, 1);
  // check that data.count_of_verticles is correct
  ck_assert_int_eq(data.count_of_verticles, 5);
  // check that data.count_of_facets is correct
  ck_assert_int_eq(data.count_of_facets, 6);
  // check that data.matrix_3d is correct
  // check that data.polygons is correct
  // check that data.array is correct
  // free memory
  remove_data(&data);
}
END_TEST

START_TEST(test_first_vertex) {
  data_t data = {0};
  char* model_file_name = "./objects/pyramid.obj";
  int result = file_parsing(&data, model_file_name);
  ck_assert_int_eq(result, 1);
  ck_assert_double_eq_tol(data.matrix_3d.matrix[1][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(data.matrix_3d.matrix[1][1], 1.0, 1e-7);
  ck_assert_double_eq_tol(data.matrix_3d.matrix[1][2], 0.0, 1e-7);
  remove_data(&data);
}
END_TEST
START_TEST(test_parsing_4) {
  data_t data = {0};
  char model_file_name[] = "./objects/nothing.obj";
  // create test_model.txt file with some test data
  int result = file_parsing(&data, model_file_name);
  ck_assert_int_eq(result, 1);
  // check that data.matrix_3d is correct
  // check that data.polygons is correct
  // check that data.array is correct
  // free memory
  remove_data(&data);
}
END_TEST

END_TEST
START_TEST(test_parsing_7) {
  data_t data = {0};
  char model_file_name[] = "./objects/text.txt";
  // create test_model.txt file with some test data
  int result = file_parsing(&data, model_file_name);
  ck_assert_int_eq(result, 1);
  // check that data.matrix_3d is correct
  // check that data.polygons is correct
  // check that data.array is correct
  // free memory
  remove_data(&data);
}
END_TEST

START_TEST(test_move_x_1) {
  data_t data;
  memset(&data, 0, sizeof(data_t));
  int result = file_parsing(&data, "./objects/pyramid.obj");
  ck_assert_int_eq(result, 1);
  double prev_x = data.array[0];
  move_x_axis(&data, 1.0, 0.0);
  ck_assert_double_eq(data.array[0], prev_x + 1.0);
  remove_data(&data);
}
END_TEST
START_TEST(test_move_y_1) {
  data_t data;
  memset(&data, 0, sizeof(data_t));
  int result = file_parsing(&data, "./objects/pyramid.obj");
  ck_assert_int_eq(result, 1);
  double prev_y = data.array[1];
  move_y_axis(&data, 1.0, 0.0);
  ck_assert_double_eq(data.array[1], prev_y + 1.0);
  remove_data(&data);
}
END_TEST
START_TEST(test_move_z_1) {
  data_t data;
  memset(&data, 0, sizeof(data_t));
  int result = file_parsing(&data, "./objects/pyramid.obj");
  ck_assert_int_eq(result, 1);
  double prev_z = data.array[2];
  move_z_axis(&data, 1.0, 0.0);
  ck_assert_double_eq(data.array[2], prev_z + 1.0);
  remove_data(&data);
}
END_TEST

START_TEST(test_rotate_x) {
  data_t data = {0};
  data.count_of_verticles = 7;
  double array[] = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                    1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0};
  data.array = array;

  rotate_x_axis(&data, 45.0, 0.0);

  // Checking if x value of verticles hasn't changed
  for (int i = 0; i < (data.count_of_verticles + 1) * 3; i += 3)
    ck_assert_double_eq_tol(data.array[i], array[i], 0.0001);

  // Checking if y and z values of vertex 1 has changed
  ck_assert_double_eq_tol(data.array[1], 0.0, 0.0001);
  ck_assert_double_eq_tol(data.array[2], 0.0, 0.0001);
}
END_TEST

START_TEST(test_rotate_y_1) {
  data_t data = {0};
  double current_angle = 1, previous_angle = 0;
  rotate_y_axis(&data, current_angle, previous_angle);
  for (int i = 0; i < data.count_of_verticles; i++) {
    double x = data.matrix_3d.matrix[i][0];
    double z = data.matrix_3d.matrix[i][2];
    ck_assert(fabs(x - (double)cos(current_angle - previous_angle)) < 0.001);
    ck_assert(fabs(z + (double)sin(current_angle - previous_angle)) < 0.001);
  }
}
END_TEST

START_TEST(test_rotate_z_1) {
  data_t data = {0};
  memset(&data, 0, sizeof(data_t));
  data.count_of_verticles = 1;
  data.array = (double*)calloc(6, sizeof(double));
  data.array[3] = 1.0;
  data.array[4] = 2.0;
  data.array[5] = 3.0;
  rotate_z_axis(&data, 45.0, 0.0);
  ck_assert_double_eq_tol(data.array[3], -(sqrt(2) / 2), 0.0001);
  ck_assert_double_eq_tol(data.array[4], 3 * (sqrt(2) / 2), 0.0001);
  ck_assert_double_eq_tol(data.array[5], 3.0, 0.0001);
  free(data.array);
}
END_TEST

START_TEST(test_rotate_z_2) {
  data_t data;
  memset(&data, 0, sizeof(data_t));
  data.count_of_verticles = 1;
  data.array = (double*)calloc(6, sizeof(double));
  data.array[3] = 1.0;
  data.array[4] = 2.0;
  data.array[5] = 3.0;
  rotate_z_axis(&data, 90.0, 45.0);
  ck_assert_double_eq_tol(data.array[3], -(sqrt(2) / 2), 0.0001);
  ck_assert_double_eq_tol(data.array[4], 3 * (sqrt(2) / 2), 0.0001);
  free(data.array);
}
END_TEST
START_TEST(test_rotate_y_3) {
  data_t data;
  memset(&data, 0, sizeof(data_t));
  data.count_of_verticles = 1;
  data.array = (double*)calloc(6, sizeof(double));
  data.array[3] = 1.0;
  data.array[4] = 2.0;
  data.array[5] = 3.0;
  rotate_y_axis(&data, 90.0, 45.0);
  ck_assert_double_eq_tol(data.array[3], 2.82843, 0.0001);
  ck_assert_double_eq_tol(data.array[4], 2, 0.0001);
  free(data.array);
}
END_TEST
START_TEST(test_scale_1) {
  data_t data = {0};
  data.count_of_verticles = 4;
  data.array = (double*)malloc(15 * sizeof(double));
  data.array[0] = 4;
  data.array[1] = -5;
  data.array[2] = 6;
  data.array[3] = 7;
  data.array[4] = -8;
  data.array[5] = 9;
  data.array[6] = 0;
  data.array[7] = 1;
  data.array[8] = -2;
  data.array[9] = -3;
  data.array[10] = 4;
  data.array[11] = -5;
  scale_array(&data, 2, 1);
  double result[12] = {8, -10, 12, 14, -16, 18, 0, 2, -4, -6, 8, -10};

  ck_assert_double_eq(data.array[0], result[0]);
  ck_assert_double_eq(data.array[1], result[1]);
  ck_assert_double_eq(data.array[2], result[2]);
  ck_assert_double_eq(data.array[3], result[3]);
  ck_assert_double_eq(data.array[4], result[4]);
  ck_assert_double_eq(data.array[5], result[5]);
  free(data.array);
}
END_TEST
int main(void) {
  Suite* s1 = suite_create("backend");
  TCase* test_case_1 = tcase_create("backend");
  SRunner* sr = srunner_create(s1);
  int wtf;
  suite_add_tcase(s1, test_case_1);

  tcase_add_test(test_case_1, test_parsing_1);
  tcase_add_test(test_case_1, test_parsing_matrix);

  tcase_add_test(test_case_1, test_parsing_3);
  tcase_add_test(test_case_1, test_first_vertex);

  tcase_add_test(test_case_1, test_parsing_4);
  tcase_add_test(test_case_1, test_parsing_7);

  tcase_add_test(test_case_1, test_move_x_1);
  tcase_add_test(test_case_1, test_move_y_1);
  tcase_add_test(test_case_1, test_move_z_1);

  tcase_add_test(test_case_1, test_rotate_x);
  tcase_add_test(test_case_1, test_rotate_y_1);
  tcase_add_test(test_case_1, test_rotate_y_3);
  tcase_add_test(test_case_1, test_rotate_z_1);
  tcase_add_test(test_case_1, test_rotate_z_2);
  tcase_add_test(test_case_1, test_scale_1);

  srunner_run_all(sr, CK_ENV);
  wtf = srunner_ntests_failed(sr);

  srunner_free(sr);
  return wtf == 0 ? 0 : 1;
}
