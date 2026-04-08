#version 410

// Inputs
in vec3 vertexPos3D;
in vec3 vertexNormal;
in vec2 vUV;

// Outputs
out vec2 uv;
out vec3 posW;
out vec3 outNormal;

// Matrices
uniform mat4 transformMat;
uniform mat4 viewMat;
uniform mat4 projMat;

// Shader main code
void main() {
	vec4 worldPos = transformMat * vec4(vertexPos3D, 1);

	posW = vec3(worldPos);
	
	outNormal = normalize(mat3(transpose(inverse(transformMat))) * vertexNormal);
	
	gl_Position = projMat * viewMat * worldPos;
	
	uv = vUV;
}
