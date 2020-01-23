#version 450

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputColor;

layout(location = 0) out vec3 fragmentColor;

void main() {
	gl_Position = vec4(inputPosition, 1.0);
	fragmentColor = inputColor;
}