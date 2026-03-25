#version 410

in vec2 TexCoords;
out vec4 color;
uniform sampler2D text;
uniform vec3 textColour;

void main(){
	// Get a pixel from texture to use as alpha in final fragment
	float alpha = texture(text, TexCoords).r;

	// Create a base colour where alpha is from texture
	// black would be 0 indicating fragment should be transparent
	// red would be 1.0 meaning fragment should be solid
	vec4 sampled = vec4 (1.0, 1.0, 1.0, alpha);

	// Determine final colour
	color = vec4(textColour, 1.0) * sampled;
}