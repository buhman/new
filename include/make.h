#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

  unsigned int make_buffer(unsigned int target,
                           const void * data,
                           size_t size);

  unsigned int make_texture(const void * data,
                            int internalformat,
                            int width,
                            int height,
                            int format,
                            int type);

#define make_buffer_sizeof(target, data) \
  make_buffer(target, data, (sizeof (data)))

#ifdef __cplusplus
}
#endif
