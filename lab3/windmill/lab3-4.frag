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
    vec3 intensity = vec3(0);
    float costheta, cosphi;

    for (int i = 0; i < 4; ++i)
    {
        if (isDirectional[0])
        {
    	    light = normalize(mat3(worldToView)*lightSourcesDirPosArr[i]);

        }   
        else 
        {
            light = mat3(worldToView)*lightSourcesDirPosArr[i];
            light = normalize(light - exPosition);
        }
    

        viewDir = -normalize(exPosition);
        normal = normalize(exNormal);
        costheta = max(dot(light,normal), 0.0);
        iDiff = normalize(lightSourcesColorArr[i]*costheta);
        iDiff = max(iDiff, 0.0);
        reflectivity = 2*normal*dot(light, normal) - light;
        cosphi = max(dot(reflectivity, viewDir), 0.0);
        iSpec = lightSourcesColorArr[i]*pow(cosphi, 80);
        iSpec = max(iSpec, 0.0);
        intensity = intensity + iDiff + iSpec;
    }

	outColor = clamp(vec4(intensity, 1.0)*vec4(texVec, 1.0),0.0,1.0);
}


