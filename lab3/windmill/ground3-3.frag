#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec2 exTexCoord;

uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.5, 0.5, 0.5);
	float shade;
	
	shade = dot(normalize(exNormal), normalize(light));
	outColor = shade*texture(texUnit, 10000*exTexCoord);
}
