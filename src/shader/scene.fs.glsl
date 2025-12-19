#version 130

precision highp float;

uniform sampler2D texture0;

in vec2 f_texture;
in vec4 f_position;

void main()
{
  vec2 coord = vec2(f_texture.x, 1.0 - f_texture.y);
  vec4 c = texture2D(texture0, coord);

  gl_FragColor = c;
  //gl_FragColor = vec4(f_position.wwww);
}
