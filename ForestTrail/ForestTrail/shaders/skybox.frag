#version 410

in vec3 texCoord;

uniform samplerCube sampler;

out vec4 fragmentColour;

void main() {
	fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);
}
