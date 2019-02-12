#version 150


in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal; // Phong

// NY
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 texVec;

void main(void)
{
	mat3 normalMatrix = mat3(modelViewMatrix); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong
    
	gl_Position = projectionMatrix * modelViewMatrix * vec4(inPosition, 1.0);
}

