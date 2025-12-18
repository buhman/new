#include <math.h>
#include <stdio.h>

#include "glad/gl.h"

#include "math/float_types.hpp"
#include "math/transform.hpp"

#include "render.h"
#include "state.hpp"

mat4x4 perspective()
{
  mat4x4 m1 = mat4x4(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 1,
                     0, 0, -1, 0
                     );
  return m1;
}

void render(unsigned int program,
            unsigned int program__trans,
            unsigned int program__texture0,
            unsigned int color,
            unsigned int vertex_array,
            int triangles_length)
{
  mat4x4 trans = perspective() * g_state.world_to_view * translate(vec3(0, 0, -2)) * scale(1.0f);

  glEnable(GL_DEPTH_TEST);
  glClearDepth(-1000.0f);
  glDepthFunc(GL_GREATER);

  glUniformMatrix4fv(program__trans, 1, GL_TRUE, &trans[0][0]);
  glUniform1i(program__texture0, 0);

  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, color);

  glBindVertexArray(vertex_array);

  glDrawElements(GL_TRIANGLES, triangles_length, GL_UNSIGNED_INT, 0);
}
