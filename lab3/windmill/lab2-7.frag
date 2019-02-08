#version 150

//in vec3 ex_Normal;
in float shade;
in vec2 ex_TexCoord;
//in mat3 normalMatrix;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
	//const vec3 light = vec3(0.8, 0.8, 0.8);
	//float shade;
    //vec3 transformedNormal;
	//transformedNormal = normalMatrix*ex_Normal;
	//shade = dot(normalize(transformedNormal), light);
	out_Color = shade*texture(texUnit, ex_TexCoord);
}
