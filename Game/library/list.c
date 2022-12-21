#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
  void **data;
  size_t size;
  size_t capacity;
  free_func_t freer;
} list_t;

const size_t RESIZE_FAC = 2;

void list_resize(list_t *list) {
  if (list->size == list->capacity) {
    list->capacity = list->capacity == 0 ? 1 : list->capacity * RESIZE_FAC;
    list->data = realloc(list->data, list->capacity * sizeof(void *));
  }
}

list_t *list_init(size_t initial_size, free_func_t freer) {
  list_t *output = malloc(sizeof(list_t));
  assert(output != NULL);
  output->data = malloc(initial_size * sizeof(void *));
  output->capacity = initial_size;
  output->size = 0;
  output->freer = freer;
  return output;
};

void list_free(list_t *list) {
  if (list->freer != NULL) {
    for (size_t i = 0; i < list->size; i++) {
      list->freer(list->data[i]);
    }
  }
  free(list->data);
  free(list);
};

size_t list_size(list_t *list) { return list->size; }

void *list_get(list_t *list, size_t index) {
  assert(index < list->size);
  return list->data[index];
}

void list_add(list_t *list, void *value) {
  list_resize(list);
  list->data[list->size] = value;
  list->size++;
}

void *list_remove(list_t *list, size_t index) {
  assert(index < list->size);
  void *removed = list_get(list, index);
  for (size_t i = index; i + 1 < list->size; i++) {
    list->data[i] = list->data[i + 1];
  }
  list->size--;
  return removed;
}

bool list_contains(list_t *list, void *value) {
  for (size_t i = 0; i < list->size; i++) {
    if (list->data[i] == value) {
      return true;
    }
  }
  return false;
}

void *list_set(list_t *list, size_t index, void *value) {
  assert(index < list->size);
  void *old_value = malloc(sizeof(void *));
  old_value = list->data[index];
  list->data[index] = value;
  return old_value;
}

bool isEmpty(list_t *list) { return list->size == 0; }