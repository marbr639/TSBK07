#version 150


out vec4 outColor;
in vec3 exNormal; // Phong

uniform vec3 texVec;

void main(void)
{
	const vec3 light = vec3(0.8, 0.8, 0.8);
	float shade;
	
	shade = dot(normalize(exNormal), normalize(light));
	outColor = vec4(shade*texVec, 1.0);
}
