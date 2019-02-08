#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;


uniform mat4 Ry;
uniform mat4 Rx;
uniform mat4 Rz;
uniform mat4 projectionMatrix;
uniform mat4 trans;
uniform mat4 camMatrix;
void main(void)
{

	gl_Position = projectionMatrix*camMatrix*trans*Ry*vec4(in_Position, 1.0);
    ex_Normal = in_Normal;
    ex_TexCoord = inTexCoord;
    
}


