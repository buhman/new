#pragma once

#ifdef __cplusplus
extern "C" {
#endif

  void render(unsigned int program,
              unsigned int program__trans,
              unsigned int program__texture0,
              unsigned int texture__color,
              const struct mesh * scene_mesh,
              const struct mesh * plane_mesh,
              const struct mesh * sphere_mesh);

#ifdef __cplusplus
}
#endif
