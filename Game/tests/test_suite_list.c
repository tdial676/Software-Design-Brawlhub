int main(int argc, char *argv[]) {
  //   list_t *array1 = list_init(1, (freer_t)vec_list_free);

  //   // // Tests the add method and the resize and size method.
  //   vec_list_t *vector1 = vec_list_init(1, (freer_t)free);
  //   vec_list_t *vector2 = vec_list_init(1, (freer_t)free);
  //   vec_list_t *vector3 = vec_list_init(1, (freer_t)free);
  //   vec_list_t *vector4 = vec_list_init(1, (freer_t)free);
  //   vec_list_t *vector5 = vec_list_init(1, (freer_t)free);

  //   assert(list_size(array1) == 0);
  //   printf("Test 0 passed\n");

  //   list_add(array1, vector1);
  //   assert(list_size(array1) == 1);
  //   assert(list_get(array1, 0) == vector1);

  //   list_add(array1, vector2);
  //   assert(list_size(array1) == 2);
  //   assert(list_get(array1, 1) == vector2);

  //   list_add(array1, vector3);
  //   assert(list_size(array1) == 3);
  //   assert(list_get(array1, 2) == vector3);

  //   list_add(array1, vector4);
  //   assert(list_size(array1) == 4);
  //   assert(list_get(array1, 3) == vector4);

  //   list_add(array1, vector5);
  //   assert(list_size(array1) == 5);
  //   assert(list_get(array1, 4) == vector5);
  //   printf("Test 1 passed\n");

  //   // Tests the get method
  //   assert(list_get(array1, 0) == vector1);
  //   assert(list_get(array1, 1) == vector2);
  //   assert(list_get(array1, 2) == vector3);
  //   assert(list_get(array1, 3) == vector4);
  //   assert(list_get(array1, 4) == vector5);
  //   printf("Test 2 passed\n");

  //   // Tests The remove and size method
  //   vec_list_t *removed = list_remove(array1, 4);
  //   assert(removed == vector5);
  //   vec_list_free(removed);
  //   assert(list_size(array1) == 4);

  //   vec_list_t *removed2 = list_remove(array1, 0);
  //   assert(removed2 == vector1);
  //   vec_list_free(removed2);
  //   assert(list_size(array1) == 3);

  //   vec_list_t *removed3 = list_remove(array1, 1);
  //   assert(removed3 == vector3);
  //   vec_list_free(removed3);
  //   assert(list_size(array1) == 2);
  //   printf("Test 3 passed\n");

  //   // Tests our free method.
  //   list_free(array1);
  //   printf("All Tests Passed :)\n");
}