#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exPosition;

uniform vec3 lightSourcesDirPosArr[1];

uniform vec3 lightSourcesColorArr[1];

uniform float specularExponent;

uniform bool isDirectional[1];

uniform mat4 worldToview;

//uniform vec3 texVec;

void main(void)
{	

    float shade;
    vec3 diffuse = vec3(0,0,0);
    float temp_diffuse = 0;

    
    vec3 light = normalize(mat3(worldToView)*lightSourcesDisPorArr[1]);
    vec3 viewDir = -normalize(exPosition);

    //shade = dot(exNormal, light);
    //shade = clamp(shade,0 ,1);    

      //  temp_diffuse = dot(exNormal, lightSourcesDirPosArr[1]);
     //   temp_diffuse = max(0.0, temp_diffuse);
     //   diffuse = temp_diffuse * lightSourcesColorArr[1];
   // shade = normalize(diffuse);
	
//	shade = dot(normalize(exNormal), normalize(light));
	outColor = vec4(shade, 1.0);
}
