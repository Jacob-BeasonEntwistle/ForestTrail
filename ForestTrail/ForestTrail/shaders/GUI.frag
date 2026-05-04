#version 410

// Input to fragment shader
in vec2 uv;

// Texture
uniform sampler2D sampler;

// Final fragment colour
out vec4 fragmentColour;

// Shader body
void main()
{
	// Flip the V texture coordinate to account for GUI screen-space origin
	// (converting top-left-origin GUI UVs to bottom-left-origin OpenGL UVs)
	vec2 guiUV = vec2(uv.x, 1.0 - uv.y);

	// Determine fragment colour from texture based on uv co-oords
	vec4 texColour = texture(sampler, guiUV);

	fragmentColour = texColour;
}
