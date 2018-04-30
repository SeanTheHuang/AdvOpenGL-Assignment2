#version 420

in vec2 eTexCoords;
out vec4 colour;
uniform sampler2D sampler;

void main()
{
	const vec4 green = vec4(0, 0.9f, 0.2f, 1);
	const vec4 brown = vec4(0.4f, 0.3f, 0.2, 1);

	colour = mix(brown, green, texture(sampler, eTexCoords.st));
}