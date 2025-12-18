#include "glad/gl.h"

#include "make.h"

unsigned int make_buffer(unsigned int target,
                         const void * data,
                         size_t size)
{
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, GL_STATIC_DRAW);

  glBindBuffer(target, 0);
  return buffer;
}

unsigned int make_texture(const void * data,
                          int internalformat,
                          int width,
                          int height,
                          int format,
                          int type)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  float color[3] = {0.0, 0.0, 0.0};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

  glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);

  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}
