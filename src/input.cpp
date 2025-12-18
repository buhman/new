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

void input()
{
  for (int i = 0; i < 16; i++) {
    GLFWgamepadstate state;
    int jid = GLFW_JOYSTICK_1 + i;
    int ret = glfwGetGamepadState(jid, &state);
    if (ret == false)
      continue;

    if (button_down(state, GLFW_GAMEPAD_BUTTON_START)) {
      for (int i = 0; i < 4; i++) {
        printf("%3.06f, %3.06f, %3.06f, %3.06f\n",
               g_state.world_to_view[i][0],
               g_state.world_to_view[i][1],
               g_state.world_to_view[i][2],
               g_state.world_to_view[i][3]);
      }
    }

    float lx = deadzone(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
    float ly = deadzone(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
    float rx = deadzone(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
    float ry = deadzone(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
    float tl = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    float tr = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
    float y = 0.01 * (tl - tr);

    g_state.world_to_view
      = rotate_x(ly * 0.01f)
      * rotate_y(lx * 0.01f)
      * translate(vec3(rx * -0.01f, y, ry * -0.01f))
      * g_state.world_to_view;

    break;
  }
}
