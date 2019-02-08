#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
//out vec3 ex_Normal;
out vec2 ex_TexCoord;
out mat3 normalMatrix;
out float shade;


uniform mat4 Ry;
uniform mat4 Rx;
uniform mat4 Rz;

uniform mat4 projectionMatrix;
uniform mat4 trans;
uniform mat4 camMatrix;
void main(void)
{

    mat4 mvMatrix = projectionMatrix*camMatrix*trans*Ry;
	gl_Position = mvMatrix*vec4(in_Position, 1.0);

    normalMatrix = mat3(mvMatrix);
    const vec3 light = vec3(0.8, 0.8, 0.8);
	
    vec3 transformedNormal;
	transformedNormal = normalMatrix*in_Normal;
	shade = dot(normalize(transformedNormal), light);
    //ex_Normal = in_Normal;
    ex_TexCoord = inTexCoord;
    
}


