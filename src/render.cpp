#include <math.h>
#include <stdio.h>

#include "glad/gl.h"

#include "math/float_types.hpp"
#include "math/transform.hpp"

#include "render.h"
#include "mesh.h"
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

void render_object(unsigned int program,
                   unsigned int program__trans,
                   unsigned int program__texture0,
                   unsigned int texture__color,
                   unsigned int vertex_array,
                   int triangles_length,
                   const mat4x4& local_to_world)
{
  mat4x4 trans = perspective() * g_state.world_to_view * local_to_world;

  glUniformMatrix4fv(program__trans, 1, GL_TRUE, &trans[0][0]);
  glUniform1i(program__texture0, 0);

  glUseProgram(program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture__color);

  glBindVertexArray(vertex_array);

  glDrawElements(GL_TRIANGLES, triangles_length, GL_UNSIGNED_INT, 0);
}

void render(unsigned int program,
            unsigned int program__trans,
            unsigned int program__texture0,
            unsigned int texture__color,
            const struct mesh * scene_mesh,
            const struct mesh * plane_mesh,
            const struct mesh * sphere_mesh)
{
  glEnable(GL_DEPTH_TEST);
  glClearDepth(-1000.0f);
  glDepthFunc(GL_GREATER);

  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_object(program,
                program__trans,
                program__texture0,
                texture__color,
                scene_mesh->vertex_array,
                scene_mesh->length,
                g_state.local_to_world.test_scene);

  render_object(program,
                program__trans,
                program__texture0,
                texture__color,
                plane_mesh->vertex_array,
                plane_mesh->length,
                g_state.local_to_world.plane);

  render_object(program,
                program__trans,
                program__texture0,
                texture__color,
                sphere_mesh->vertex_array,
                sphere_mesh->length,
                g_state.local_to_world.sphere);
}
