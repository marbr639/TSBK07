#version 150

in  vec3 in_Position;
in vec3 in_Normal;
out vec3 ex_Normal;

uniform mat4 Ry;
uniform mat4 Rx;
uniform mat4 Rz;
void main(void)
{

	gl_Position = Rx*Ry*Rz* vec4(in_Position, 1.0);
    ex_Normal = in_Normal;
    
}


