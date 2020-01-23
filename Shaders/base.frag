#version 450

layout(set = 0, binding = 0) uniform ColorUniform
{
	vec3 color;
} colorUniform;


layout(location = 0) in vec3 fragmentColor;
layout(location = 0) out vec4 fragColor;

void main() 
{
	fragColor = vec4(colorUniform.color, 1.0f);
}