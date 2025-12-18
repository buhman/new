#include <stdio.h>

#include "glad/gl.h"

#include "shader.h"

unsigned int compile_shader(const char * name,
                            const void * vs,
                            const int vs_length,
                            const void * fs,
                            const int fs_length)
{
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const char **)&vs, &vs_length);
  glCompileShader(vertex_shader);
  {
    int  success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      fprintf(stderr, "%s: vertex shader compile failed:\n%s\n", name, info_log);
    }
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const char **)&fs, &fs_length);
  glCompileShader(fragment_shader);
  {
    int  success;
    char info_log[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      fprintf(stderr, "%s: fragment shader compile failed:\n%s\n", name, info_log);
    }
  }

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glBindAttribLocation(shader_program, shader_attrib_position, "v_position");
  glBindAttribLocation(shader_program, shader_attrib_texture, "v_texture");
  glBindAttribLocation(shader_program, shader_attrib_normal, "v_normal");
  glLinkProgram(shader_program);

  {
    int  success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      fprintf(stderr, "%s: program link failed:\n%s\n", name, info_log);
    }
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}
