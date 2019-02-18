#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exPosition;

uniform vec3 lightSourcesDirPosArr[4];

uniform vec3 lightSourcesColorArr[4];

uniform float specularExponent;

uniform bool isDirectional[4];

uniform mat4 worldToView;

uniform vec3 texVec;

void main(void)
{
    vec3 light, iDiff, iSpec, viewDir, reflectivity, normal;
    vec3 intensityDiff = vec3(0);
    vec3 intensitySpec = vec3(0);
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
        iDiff = lightSourcesColorArr[i]*costheta;
        iDiff = max(iDiff, 0.0);

        viewDir = -normalize(exPosition);
        reflectivity = 2*normal*dot(light, normal) - light;
        cosphi = max(dot(reflectivity, viewDir), 0.0);
        iSpec = lightSourcesColorArr[i]*pow(cosphi, specularExponent);
        intensityDiff = intensityDiff + iDiff;
        intensitySpec = intensitySpec + iSpec;
    }

    
	outColor = vec4(0.5*intensityDiff + 0.5*intensitySpec,1.0);
}


