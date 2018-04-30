#version 420

in vec2 texCoords;
out vec4 colour;
uniform sampler2D sampler;

void main()
{
	colour = texture(sampler, texCoords.st);
}