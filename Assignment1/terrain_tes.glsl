#version 430 core

uniform mat4 mvp;

in vec2 cTexCoords[];
out vec2 eTexCoords;
uniform sampler2D sampler;

layout(quads, equal_spacing, ccw) in;

void main()
{
	const float heightIntensity = 15;

	vec4 p1 = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 pos = mix(p1, p2, gl_TessCoord.y);

	vec2 tex1 = mix(cTexCoords[1], cTexCoords[0], gl_TessCoord.x);
	vec2 tex2 = mix(cTexCoords[2], cTexCoords[3], gl_TessCoord.x);
	vec2 finalTex = mix(tex1, tex2, gl_TessCoord.y);
	pos.y += texture(sampler, finalTex.ts).r * heightIntensity;

	gl_Position = mvp * pos;
	eTexCoords = finalTex;
}