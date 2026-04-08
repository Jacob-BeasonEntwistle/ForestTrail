#version 410

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec3 vertexNormal;
in vec2 vUV;

// Transformation matrix
in mat4 instance_transform_mat;

// Output
out vec2 uv;
out vec3 posW;
out vec3 outNormal;

// View project matrices.  Note no model matrix as each instance
// has its own matrix
uniform mat4 viewMat;
uniform mat4 projMat;

// Shader main code
void main() {
	vec4 worldPos = instance_transform_mat * vec4(vertexPos3D, 1);

	posW = vec3(worldPos);
	
	outNormal = normalize(mat3(transpose(inverse(instance_transform_mat))) * vertexNormal);
	
	gl_Position = projMat * viewMat * worldPos;
	
	uv = vUV;
}
