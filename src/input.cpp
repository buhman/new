#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "math/transform.hpp"

#include "input.h"
#include "state.hpp"

static inline float deadzone(float n)
{
  if (fabsf(n) < 0.05f) {
    return 0;
  } else {
    return n;
  }
}

static inline bool button_down(const GLFWgamepadstate& state, int i)
{
  bool down = state.buttons[i] && (g_state.button[i] != state.buttons[i]);
  g_state.button[i] = state.buttons[i];
  return down;
}

void print_matrix(const char * name, const mat4x4& mat)
{
  printf("  .%s = mat4x4(\n", name);
  for (int i = 0; i < 4; i++) {
    printf("    % 3.06f, % 3.06f, % 3.06f, % 3.06f%s\n",
           mat[i][0],
           mat[i][1],
           mat[i][2],
           mat[i][3],
           (i == 3) ? "" : ",");
  }
  printf("  ),\n");
}

void input()
{
  for (int i = 0; i < 16; i++) {
    GLFWgamepadstate state;
    int jid = GLFW_JOYSTICK_1 + i;
    int ret = glfwGetGamepadState(jid, &state);
    if (ret == false)
      continue;

    mat4x4& local_to_world = g_state.local_to_world.plane;

    if (button_down(state, GLFW_GAMEPAD_BUTTON_START)) {
      if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) {
        print_matrix("local_to_world", local_to_world);
      } else {
        print_matrix("world_to_view", g_state.world_to_view);
      }
    }

    float lx = deadzone(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
    float ly = deadzone(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
    float rx = deadzone(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
    float ry = deadzone(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
    float tl = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    float tr = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
    float y = 0.01 * (tl - tr);

    bool up = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] != 0;
    bool down = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] != 0;
    float s = up ? 1.01 : (down ? 0.99 : 1.0);

    if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) {
      local_to_world
        = translate(vec3(rx * -0.01f, y, ry * -0.01f))
        * local_to_world
        * rotate_x(ly * 0.01f)
        * rotate_y(lx * 0.01f)
        * scale(s);
    } else {
      g_state.world_to_view
        = rotate_x(ly * 0.01f)
        * rotate_y(lx * 0.01f)
        * translate(vec3(rx * -0.01f, y, ry * -0.01f))
        * g_state.world_to_view;
    }

    break;
  }
}
