#version 330 core
uniform mat4 transform;

layout (location = 0) in vec3 aPos;
void main() {
	vec4 pos = transform * vec4(aPos, 1.);
	gl_Position = pos;
}