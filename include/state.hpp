#pragma once

#include "math/float_types.hpp"

struct state {
  mat4x4 world_to_view;
  int button[16];
};

extern state g_state;
