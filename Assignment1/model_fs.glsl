#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 mWorldPos;

out vec4 color;

uniform sampler2D texture_diffuse0;
uniform vec3 modelColour;
uniform vec3 cameraPos;

uniform vec3 fogColour;
uniform float fogStart;
uniform float fogEnd;

void main(){
	float ambientIntensity = 0.7f;
	float specularIntensity = 2.0f;
	float diffuseIntensity = 1.0f;

	//Set colour, apply fog
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - fogStart)/ fogEnd;
	lerp = clamp(lerp, 0.0f, 1.0f);

	vec3 usedColour = texture(texture_diffuse0, TexCoords).rgb * modelColour;
	usedColour = mix(usedColour, fogColour, lerp);


	//Do lighting stuff
	vec3 lightPos = vec3(50,0,-30);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + cameraPos);  

    float diff = max(dot(lightDir, norm), 0.0) * diffuseIntensity;
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0) * specularIntensity;

	vec3 ambient = ambientIntensity * usedColour;
	vec3 diffuse = diff * usedColour;
	vec3 specular = vec3(specularIntensity) * spec;

	//Apply lighting stuff
	color =  vec4(ambient + diffuse + specular, 1.0);
}