#version 150


in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal; // Phong

// NY
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 projectionMatrix;
uniform vec3 texVec;

void main(void)
{
	mat3 normalMatrix = mat3(worldToView*modelToWorld); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong
    
	gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(inPosition, 1.0);
}

