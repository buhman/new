#pragma once

#ifdef __cplusplus
extern "C" {
#endif

  unsigned int compile_shader(const char * name,
                              const void * vs,
                              const int vs_length,
                              const void * fs,
                              const int fs_length);

  static const int shader_attrib_position = 0;
  static const int shader_attrib_texture = 1;
  static const int shader_attrib_normal = 2;

#ifdef __cplusplus
}
#endif
