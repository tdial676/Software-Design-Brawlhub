#include "forces.h"
#include "collision.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int MINIMUM_DISTANCE = 5;
const size_t RADIUS = 10;

typedef struct collision_force_aux {
  body_t *body1;
  body_t *body2;
  collision_handler_t handler;
  void *handler_aux;
  bool colliding;
  free_func_t freer;
} collision_force_aux_t;

typedef struct aux_two {
  body_t *body1;
  body_t *body2;
  double constant;
} aux_two_t;

typedef struct aux_one {
  body_t *body;
  double constant;
} aux_one_t;

void free_collision_aux(collision_force_aux_t *aux) {
  if (aux->freer != NULL) {
    aux->freer(aux->handler_aux);
  }
  free(aux);
}

void calculate_gravity(void *aux) {
  aux_two_t *aux_n = aux;
  body_t *body1 = aux_n->body1;
  body_t *body2 = aux_n->body2;
  double constant = aux_n->constant;
  vector_t distance_v =
      vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
  double distance_d = pow(pow(distance_v.x, 2) + pow(distance_v.y, 2), .5);
  double constForce = -1 * constant * body_get_mass(body1) *
                      body_get_mass(body2) / pow(distance_d, 2);
  if (distance_d >= MINIMUM_DISTANCE) {
    distance_v.x = distance_v.x / distance_d;
    distance_v.y = distance_v.y / distance_d;
    vector_t force = vec_multiply(constForce, distance_v);
    body_add_force(body1, vec_negate(force));
    body_add_force(body2, force);
  }
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  aux_two_t *aux = malloc(sizeof(aux_two_t));
  aux->constant = G;
  aux->body1 = body1;
  aux->body2 = body2;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)calculate_gravity, aux,
                                 bodies, (free_func_t)free);
}

void calculate_spring(void *aux) {
  aux_two_t *aux_n = aux;
  body_t *body1 = aux_n->body1;
  body_t *body2 = aux_n->body2;
  double constant = aux_n->constant;
  vector_t distance_v =
      vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
  vector_t force = vec_multiply(-constant, distance_v);
  body_add_force(body1, vec_negate(force));
  body_add_force(body2, force);
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  aux_two_t *aux = malloc(sizeof(aux_two_t));
  aux->constant = k;
  aux->body1 = body1;
  aux->body2 = body2;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)calculate_spring, aux,
                                 bodies, (free_func_t)free);
}

void calculate_drag(void *aux) {
  aux_one_t *aux_n = aux;
  body_t *body = aux_n->body;
  double constant = aux_n->constant;
  vector_t force = vec_multiply(-1 * constant, body_get_velocity(body));
  body_add_force(body, force);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
  aux_one_t *aux = malloc(sizeof(aux_one_t));
  aux->constant = gamma;
  aux->body = body;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body);
  scene_add_bodies_force_creator(scene, (force_creator_t)calculate_drag, aux,
                                 bodies, (free_func_t)free);
}

void apply_collision(void *aux) {
  collision_force_aux_t *aux_n = aux;
  body_t *body1 = aux_n->body1;
  body_t *body2 = aux_n->body2;
  collision_info_t info =
      find_collision(body_get_shape(body1), body_get_shape(body2));
  bool collision_state = aux_n->colliding;
  if (info.collided) {
    if (body_get_mass(body1) != INFINITY && body_get_mass(body2) != INFINITY) {
      body_set_centroid(body1, vec_subtract(body_get_centroid(body1),
                                            vec_multiply(0.5, info.axis)));
      body_set_centroid(body2, vec_add(body_get_centroid(body1),
                                       vec_multiply(0.5, info.axis)));
    } else if (body_get_mass(body1) == INFINITY) {
      body_set_centroid(body2, vec_add(body_get_centroid(body1), info.axis));
    } else if (body_get_mass(body2) == INFINITY) {
      body_set_centroid(body1,
                        vec_subtract(body_get_centroid(body1), info.axis));
    }
  }
  if (info.collided && !collision_state) {
    aux_n->handler(body1, body2, info.axis, aux_n->handler_aux);
    aux_n->colliding = true;
  } else if (!info.collided) {
    aux_n->colliding = false;
  }
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2,
                      collision_handler_t handler, void *aux,
                      free_func_t freer) {
  collision_force_aux_t *aux_n = malloc(sizeof(collision_force_aux_t));
  aux_n->body1 = body1;
  aux_n->body2 = body2;
  aux_n->handler = handler;
  aux_n->handler_aux = aux;
  aux_n->colliding = false;
  aux_n->freer = freer;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)apply_collision, aux_n,
                                 bodies, (free_func_t)free_collision_aux);
}

void destroy_bodies(body_t *body1, body_t *body2, vector_t axis, void *aux) {
  bool *destroy_both = (bool *)aux;
  body_remove(body2);
  if (*destroy_both) {
    body_remove(body1);
  }
}

void create_destructive_collision(scene_t *scene, body_t *body1,
                                  body_t *body2) {
  bool *destroy_both = malloc(sizeof(bool));
  *destroy_both = true;
  create_collision(scene, body1, body2, (collision_handler_t)destroy_bodies,
                   destroy_both, (free_func_t)free);
}

void create_solo_destructive_collision(scene_t *scene, body_t *retained,
                                       body_t *removed) {
  bool *destroy_both = malloc(sizeof(bool));
  *destroy_both = false;
  create_collision(scene, retained, removed,
                   (collision_handler_t)destroy_bodies, destroy_both,
                   (free_func_t)free);
}

void damage_body(body_t *damaged, body_t *removed, vector_t axis, void *aux) {
  bullet_info_t *damager = (bullet_info_t *)aux;
  body_add_damage(damaged, (double)(int)*damager);
  body_remove(removed);
}

void create_damaging_collision(scene_t *scene, body_t *damaged,
                               body_t *removed) {
  bullet_info_t *bullet_info = (bullet_info_t *)body_get_info(removed);
  create_collision(scene, damaged, removed, (collision_handler_t)damage_body,
                   bullet_info, NULL);
}

void Phy_collision_handler(body_t *body1, body_t *body2, vector_t axis,
                           void *aux) {
  double mass_1 = body_get_mass(body1);
  double mass_2 = body_get_mass(body2);
  double reduced_mass = 0.0;
  if (mass_1 == INFINITY) {
    reduced_mass = mass_2;
  } else if (mass_2 == INFINITY) {
    reduced_mass = mass_1;
  } else {
    reduced_mass = (mass_1 * mass_2) / (mass_1 + mass_2);
  }
  double *elasticity = (double *)aux;
  double u1 = vec_dot(axis, body_get_velocity(body1));
  double u2 = vec_dot(axis, body_get_velocity(body2));
  double impulse = reduced_mass * (1 + *elasticity) * (u2 - u1);
  body_add_impulse(body1, vec_multiply(impulse, (axis)));
  body_add_impulse(body2, vec_multiply(impulse, vec_negate(axis)));
}

void create_physics_collision(scene_t *scene, double elasticity, body_t *body1,
                              body_t *body2) {
  double *elasticity_ptr = malloc(sizeof(double));
  *elasticity_ptr = elasticity;
  create_collision(scene, body1, body2,
                   (collision_handler_t)Phy_collision_handler, elasticity_ptr,
                   (free_func_t)free);
}