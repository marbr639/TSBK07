#version 150


in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec3 exNormal; // Phong
out vec3 exPosition;
out vec2 exTexCoord;

// NY
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 projectionMatrix;


void main(void)
{
	mat3 normalMatrix = mat3(worldToView*modelToWorld); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong
    exPosition = vec3(worldToView*modelToWorld*vec4(inPosition,1.0)); // For interpolation of vertex positions
    exTexCoord = inTexCoord;
    
	gl_Position = projectionMatrix * worldToView* modelToWorld * vec4(inPosition, 1.0);
}

