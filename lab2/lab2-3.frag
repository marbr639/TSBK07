#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.8, 0.8, 0.8);
	float shade;
	
	shade = dot(normalize(ex_Normal), light);
	out_Color = texture(texUnit, ex_TexCoord);
}
