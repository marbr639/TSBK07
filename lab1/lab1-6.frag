#version 150

in vec3 ex_Normal;
out vec4 out_Color;

void main(void)
{
    const vec3 light = vec3(0.0, 1.0, 0.0);
	float shade;
	
	shade = dot(normalize(ex_Normal), light);
	out_Color = vec4(shade, shade, 0.8, 1.0);
}
