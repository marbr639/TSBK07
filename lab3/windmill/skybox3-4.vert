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
    
	gl_Position = projectionMatrix * worldToView * modelToWorld * vec4(inPosition, 1.0);

    exTexCoord = inTexCoord;
}

