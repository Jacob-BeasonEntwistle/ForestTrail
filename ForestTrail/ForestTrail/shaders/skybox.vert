#version 410

in vec3 vertexPos3D;

out vec3 texCoord;

uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
	vec4 v = vec4(vertexPos3D.xyz, 1);

	v = projMat * viewMat * v;
	
	gl_Position = v;
	
	texCoord = vertexPos3D;
}
