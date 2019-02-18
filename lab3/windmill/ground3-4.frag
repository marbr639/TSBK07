#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exPosition;
in vec2 exTexCoord;

uniform vec3 lightSourcesDirPosArr[4];

uniform vec3 lightSourcesColorArr[4];

uniform float specularExponent;

uniform bool isDirectional[4];

uniform mat4 worldToView;

uniform sampler2D texUnit;

void main(void)
{
    vec3 light, iDiff, normal;
    vec3 intensity = vec3(0);
    vec4 texVec = vec4(0);
    float costheta, cosphi;

    for (int i = 0; i < 4; ++i)
    {
        if (isDirectional[i])
        {
    	    light = normalize(mat3(worldToView)*lightSourcesDirPosArr[i]);

        }   
        else 
        {
            light = mat3(worldToView)*lightSourcesDirPosArr[i];
            light = normalize(light - exPosition);
        }
    

        normal = normalize(exNormal);
        costheta = max(dot(light,normal), 0.0);
        iDiff = normalize(lightSourcesColorArr[i]*costheta);
        iDiff = max(iDiff, 0.0);
      
        intensity = intensity + iDiff;
    }

    texVec = texture(texUnit, 10000*exTexCoord);
	outColor = clamp(vec4(intensity, 1.0)*texVec,0.0,1.0);
}


