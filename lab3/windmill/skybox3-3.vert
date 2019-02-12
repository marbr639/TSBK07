#version 150


in  vec3 inPosition;
// NY
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec2 inTexCoord;
out vec2 exTexCoord;

void main(void)
{
    
	gl_Position = projectionMatrix * modelViewMatrix * vec4(inPosition, 1.0);

    exTexCoord = inTexCoord;
}

