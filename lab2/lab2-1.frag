#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;

uniform mat2 TexMatrix;

void main(void)
{

    vec2 texVec;
	float a = sin(ex_TexCoord.s*30)/2+0.5;
    float b = sin(ex_TexCoord.t*30)/2+0.5;
    texVec = vec2(a,b);
    out_Color = vec4(texVec, 0.5, 0.0);
}
