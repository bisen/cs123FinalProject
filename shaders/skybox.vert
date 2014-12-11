#version 400

in vec3 position;
uniform mat4 v;
out vec3 texcoords;

void main () {
  texcoords = position;
  gl_Position =  v * vec4 (position, 1.0);
}
