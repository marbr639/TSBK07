#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
//out vec3 ex_Normal;
out vec2 ex_TexCoord;
out mat3 normalMatrix;
out float shade;


//uniform mat4 rot;
//uniform mat4 Rx;
//uniform mat4 Rz;

uniform mat4 projectionMatrix;
//uniform mat4 trans;
uniform mat4 modelViewMatrix;
void main(void)
{

    mat4 mvMatrix = projectionMatrix*modelViewMatrix;
	gl_Position = mvMatrix*vec4(inPosition, 1.0);

    normalMatrix = mat3(mvMatrix);
    const vec3 light = vec3(0.8, 0.8, 0.8);
	
    vec3 transformedNormal;
	transformedNormal = normalMatrix*inNormal;
	shade = dot(normalize(transformedNormal), light);
    //ex_Normal = inNormal;
    ex_TexCoord = inTexCoord;
    
}


