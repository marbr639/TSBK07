#version 150


out vec4 outColor;
in vec2 exTexCoord;

uniform sampler2D texUnit;

void main(void)
{
	vec4 color = texture(texUnit, exTexCoord);
	outColor = clamp(color,0,1);
}
