#version 420 core

out vec4 color;
in vec2 TexCoords;

const float offset = 1.0 / 300.0;
const float cutOff = 3;
uniform sampler2D screenTexture;
uniform float ifEdgeDetection;

void main()
{
	// Sobel Kernel

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), // top-left
		vec2(0.0f, offset), // top-center
		vec2(offset, offset), // top-right
		vec2(-offset, 0.0f),   // center-left
		vec2(0.0f, 0.0f),   // center-center
		vec2(offset, 0.0f),   // center-right
		vec2(-offset, -offset), // bottom-left
		vec2(0.0f, -offset), // bottom-center
		vec2(offset, -offset)  // bottom-right    
		);

	float vKernel[9] = float[](
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
		);

	float hKernel[9] = float[](
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
		);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	
	float hVal, vVal, totalVal;
	for (int i = 0; i < 9; i++)
	{
		hVal += (sampleTex[i].r + sampleTex[i].g + sampleTex[i].b) * hKernel[i];
		vVal += (sampleTex[i].r + sampleTex[i].g + sampleTex[i].b) * vKernel[i];
	}

	totalVal = sqrt(hVal * hVal + vVal * vVal) / cutOff;
	totalVal = clamp(totalVal, 0, 1);

	color = vec4(vec3(totalVal), 1.0f);
	color = mix(vec4(sampleTex[4], 1.0), color, ifEdgeDetection);
}