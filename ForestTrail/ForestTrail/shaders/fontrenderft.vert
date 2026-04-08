#version 410

// Vertex input
in vec4 vertex;
out vec2 TexCoords;

// Only a projection matrix is necessary
uniform mat4 projectionMatrix;

void main(){
	// Transform vertex with respect to projection
	gl_Position = projectionMatrix * vec4 (vertex.xy, 0.0, 1.0);

	// Texture coords are determined by z and w value of vertex
	TexCoords = vertex.zw;
}
