#version 420

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textureCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(a_position, 1.0f);
}