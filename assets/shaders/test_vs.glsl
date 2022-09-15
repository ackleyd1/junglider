#version 430

in vec4 pos;
out vec2 coords;

void main() {
  gl_Position = vec4(pos.xy, 0.0, 1.0);
  coords = pos.zw;
}