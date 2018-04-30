#version 420

in vec2 eTexCoords;
out vec4 colour;
uniform sampler2D sampler;

void main()
{
	colour = texture(sampler, eTexCoords.st);
}