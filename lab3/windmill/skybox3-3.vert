#version 150


in  vec3 inPosition;
// NY
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 projectionMatrix;

in vec2 inTexCoord;
out vec2 exTexCoord;

void main(void)
{
    
    mat3 newWorldToView = mat3(worldToView);
	gl_Position = projectionMatrix * mat4(newWorldToView) * modelToWorld * vec4(inPosition, 1.0);

    exTexCoord = inTexCoord;
}

