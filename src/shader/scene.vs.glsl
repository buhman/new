#version 130

in vec3 v_position;
in vec2 v_texture;

uniform mat4 trans;

out vec2 f_texture;
out vec4 f_position;

void main()
{
  vec4 pos = trans * vec4(v_position, 1.0);
  gl_Position = pos;
  f_texture = v_texture;
  f_position = pos;
}
