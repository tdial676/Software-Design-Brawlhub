#include "scene.h"
#include "body.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct force {
  force_creator_t forcer;
  void *aux;
  list_t *bodies;
  free_func_t freer;
} force_t;

typedef struct scene {
  list_t *bodies;
  list_t *forces;
} scene_t;

const size_t INITIAL_SIZE = 10;

void free_force(force_t *force) {
  if (force->freer != NULL) {
    force->freer(force->aux);
  }
  if (force->bodies != NULL) {
    list_free(force->bodies);
  }
  free(force);
}

scene_t *scene_init(void) {
  scene_t *init_scene = malloc(sizeof(scene_t));
  assert(init_scene != NULL);
  init_scene->bodies = list_init(INITIAL_SIZE, (free_func_t)body_free);
  init_scene->forces = list_init(INITIAL_SIZE, (free_func_t)free_force);
  return init_scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->forces);
  list_free(scene->bodies);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return list_size(scene->bodies); }

body_t *scene_get_body(scene_t *scene, size_t index) {
  return (body_t *)(list_get(scene->bodies, index));
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
  assert(index >= 0 && scene_bodies(scene) > index);
  body_remove(scene_get_body(scene, index));
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies,
                                    free_func_t freer) {
  force_t *force = malloc(sizeof(force_t));
  force->forcer = forcer;
  force->aux = aux;
  force->bodies = bodies;
  force->freer = freer;
  list_add(scene->forces, force);
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux,
                             free_func_t freer) {
  scene_add_bodies_force_creator(scene, forcer, aux, NULL, freer);
}

void scene_tick(scene_t *scene, double dt) {
  for (size_t i = 0; i < list_size(scene->forces); i++) {
    force_t *force = list_get(scene->forces, i);
    force->forcer(force->aux);
  }

  for (size_t i = 0; i < list_size(scene->forces); i++) {
    force_t *force = list_get(scene->forces, i);
    if (force->bodies != NULL) {
      for (size_t j = 0; j < list_size(force->bodies); j++) {
        body_t *body_to_check = list_get(force->bodies, j);
        if (body_is_removed(body_to_check)) {
          list_remove(scene->forces, i);
          free_force(force);
          i--;
          break;
        }
      }
    }
  }

  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_t *body = list_get(scene->bodies, i);
    if (body_is_removed(body)) {
      list_remove(scene->bodies, i);
      body_free(body);
      i--;
    }
  }

  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_tick(scene_get_body(scene, i), dt);
  }
}

list_t *scene_bodies_with_comp_info(scene_t *scene, computer_info_t info) {
  list_t *bodies = list_init(30, NULL);
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_t *body = scene_get_body(scene, i);
    computer_info_t *b_info = body_get_info(body);
    if (*b_info == info) {
      list_add(bodies, scene_get_body(scene, i));
    }
  }
  return bodies;
}

list_t *scene_bodies_with_bullet_info(scene_t *scene, bullet_info_t info) {
  list_t *bodies = list_init(30, NULL);
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_t *body = scene_get_body(scene, i);
    bullet_info_t *b_info = body_get_info(body);
    if (*b_info == info) {
      list_add(bodies, scene_get_body(scene, i));
    }
  }
  return bodies;
}