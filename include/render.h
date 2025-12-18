#pragma once

#ifdef __cplusplus
extern "C" {
#endif

  void render(unsigned int program,
              unsigned int program__trans,
              unsigned int program__texture0,
              unsigned int color,
              unsigned int vertex_array,
              int triangles_length);

#ifdef __cplusplus
}
#endif
