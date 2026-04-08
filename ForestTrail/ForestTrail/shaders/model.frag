#version 410

// Inputs
in vec2 uv;
in vec3 outNormal;
in vec3 posW;

// Lighting variables
uniform vec3 lightColour;
const vec3 ambient = vec3(0.35f, 0.35f, 0.35f);
const float shininess = 32.0f;
const float specularStrength = 0.2f;

uniform vec3 viewPos;
uniform sampler2D sampler;

// Output
out vec4 fragmentColour;

void main() {
	vec4 texColour = texture(sampler, uv).rgba;
	// Create a light direction (for directional light) instead of light position (for point light)
	const vec3 lightDir = normalize(vec3(1.0f, -1.0f, -1.0f));
	vec3 normalizedNormal = normalize(outNormal);
	//vec3 lightDirection = normalize(lightPos - posW);
	vec3 L = normalize(-lightDir);
	float diffIllum = max(dot(normalizedNormal, L), 0.0f);
	vec3 diffuse = diffIllum * lightColour;
	vec3 viewDir = normalize(viewPos - posW);
	vec3 reflectDirection = reflect(-L, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), shininess);
	vec3 specular = specularStrength * spec * lightColour;
	vec3 finalColour = ambient * texColour.rgb + diffuse * texColour.rgb + specular;
	fragmentColour = vec4(finalColour, texColour.a);
}
