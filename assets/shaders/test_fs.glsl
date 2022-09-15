#version 430

in vec2 coords;
uniform sampler2D tex;
out vec4 color;

void main() {
  color = texture(tex, coords);
}