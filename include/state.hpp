#pragma once

#include "math/float_types.hpp"

struct state {
  mat4x4 world_to_view;
  struct {
    mat4x4 test_scene;
    mat4x4 plane;
    mat4x4 sphere;
  } local_to_world;
  int button[16];
};

extern state g_state;
