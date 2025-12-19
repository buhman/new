#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include "mesh.h"
#include "make.h"
#include "shader.h"
#include "render.h"
#include "input.h"
#include "model/test_scene.h"
#include "model/plane.h"
#include "model/sphere.h"
#include "model/palette.data.h"
#include "shader/scene.vs.glsl.h"
#include "shader/scene.fs.glsl.h"

typedef unsigned int uint;

int vp_width = 800;
int vp_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  vp_width = width;
  vp_height = height;
}

uint vertex_array_attributes(uint vertex_buffer,
                             uint index_buffer)
{
  uint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  glVertexAttribPointer(shader_attrib_position,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        (sizeof (float)) * 8,
                        (void*)(0 * 4)
                        );
  glVertexAttribPointer(shader_attrib_texture,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        (sizeof (float)) * 8,
                        (void*)(3 * 4)
                        );
  glVertexAttribPointer(shader_attrib_normal,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        (sizeof (float)) * 8,
                        (void*)(5 * 4)
                        );
  glEnableVertexAttribArray(shader_attrib_position);
  glEnableVertexAttribArray(shader_attrib_texture);
  glEnableVertexAttribArray(shader_attrib_normal);

  glBindVertexArray(0);

  return vertex_array;
}

int main()
{
  //////////////////////////////////////////////////////////////////////
  // window initialization
  //////////////////////////////////////////////////////////////////////

#ifdef __linux__
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

  GLFWwindow* window = glfwCreateWindow(vp_width, vp_height, "new", NULL, NULL);
  if (window == NULL) {
    const char* description;
    glfwGetError(&description);
    fprintf(stderr, "Failed to create GLFW window: %s\n", description);
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "gladLoadGL error\n");
    return -1;
  }
  if (!GLAD_GL_VERSION_2_1) {
    fprintf(stderr, "error: OpenGL version: %d.%d\n",
            GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    return -1;
  }

  //////////////////////////////////////////////////////////////////////
  // meshes
  //////////////////////////////////////////////////////////////////////

  struct mesh scene_mesh;
  scene_mesh.vtx = make_buffer_sizeof(GL_ARRAY_BUFFER, test_scene_vertices);
  scene_mesh.idx = make_buffer_sizeof(GL_ELEMENT_ARRAY_BUFFER, test_scene_Plane_triangles);
  scene_mesh.length = test_scene_Plane_triangles_length;
  scene_mesh.vertex_array = vertex_array_attributes(scene_mesh.vtx, scene_mesh.idx);

  struct mesh plane_mesh;
  plane_mesh.vtx = make_buffer_sizeof(GL_ARRAY_BUFFER, plane_vertices);
  plane_mesh.idx = make_buffer_sizeof(GL_ELEMENT_ARRAY_BUFFER, plane_Plane_triangles);
  plane_mesh.length = plane_Plane_triangles_length;
  plane_mesh.vertex_array = vertex_array_attributes(plane_mesh.vtx, plane_mesh.idx);

  struct mesh sphere_mesh;
  sphere_mesh.vtx = make_buffer_sizeof(GL_ARRAY_BUFFER, sphere_vertices);
  sphere_mesh.idx = make_buffer_sizeof(GL_ELEMENT_ARRAY_BUFFER, sphere_Sphere_triangles);
  sphere_mesh.length = sphere_Sphere_triangles_length;
  sphere_mesh.vertex_array = vertex_array_attributes(sphere_mesh.vtx, sphere_mesh.idx);

  //////////////////////////////////////////////////////////////////////
  // shaders
  //////////////////////////////////////////////////////////////////////

  uint program = compile_shader("scene",
                                src_shader_scene_vs_glsl_start,
                                src_shader_scene_vs_glsl_size,
                                src_shader_scene_fs_glsl_start,
                                src_shader_scene_fs_glsl_size);
  uint program__trans = glGetUniformLocation(program, "trans");
  uint program__texture0 = glGetUniformLocation(program, "texture0");

  //////////////////////////////////////////////////////////////////////
  // textures
  //////////////////////////////////////////////////////////////////////

  uint texture_palette = make_texture(model_palette_data_start,
                                      GL_RGB,
                                      32,
                                      32,
                                      GL_RGB,
                                      GL_UNSIGNED_BYTE);

  //////////////////////////////////////////////////////////////////////
  // framebuffer
  //////////////////////////////////////////////////////////////////////

  uint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  uint framebuffer__color = make_texture(NULL, GL_RGBA, vp_width, vp_height, GL_RGBA, GL_UNSIGNED_BYTE);
  uint framebuffer__depth = make_texture(NULL, GL_DEPTH24_STENCIL8, vp_width, vp_height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer__color, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, framebuffer__depth, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //////////////////////////////////////////////////////////////////////
  // main loop
  //////////////////////////////////////////////////////////////////////

  const double frame_rate = 60.0;
  const double first_frame = glfwGetTime();
  double last_frame = first_frame;

  while (!glfwWindowShouldClose(window)) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    input();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    render(program,
           program__trans,
           program__texture0,
           texture_palette,
           &scene_mesh,
           &plane_mesh,
           &sphere_mesh);

    glfwSwapBuffers(window);
    glfwPollEvents();

    //////////////////////////////////////////////////////////////////////
    // wait for the next frame
    //////////////////////////////////////////////////////////////////////

    double next_frame = last_frame + 1.0 / frame_rate;
    last_frame = glfwGetTime();
    while (next_frame - glfwGetTime() > 0) {
      double delta = next_frame - glfwGetTime();
      if (delta < 0)
        break;
      glfwWaitEventsTimeout(delta);
    }
    //break;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  uint32_t buf[800 * 600];
  glReadPixels(0, 0, vp_width, vp_height,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               buf);
  FILE * depth_f = fopen("depth.data", "wb");
  assert(depth_f != NULL);
  fwrite(buf, 4, 800 * 600, depth_f);
  fclose(depth_f);
}
