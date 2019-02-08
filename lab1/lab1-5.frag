#version 150

in vec3 ex_color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ex_color, 1.0);
}
