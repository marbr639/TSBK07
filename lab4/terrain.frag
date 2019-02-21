#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exPosition;
in vec2 exTexCoord;

uniform vec3 lightSourcesDirPosArr[4];

uniform vec3 lightSourcesColorArr[4];

uniform bool isDirectional[4];

uniform mat4 worldView;

uniform sampler2D tex;

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
    	    light = normalize(mat3(worldView)*lightSourcesDirPosArr[i]);

        }   
        else 
        {
            light = mat3(worldView)*lightSourcesDirPosArr[i];
            light = normalize(light - exPosition);
        }
    

        normal = normalize(exNormal);
        costheta = max(dot(light,normal), 0.0);
        iDiff = lightSourcesColorArr[i]*costheta;
        iDiff = max(iDiff, 0.0);
      
        intensity = intensity + iDiff;
    }

    texVec = texture(tex, exTexCoord);
	outColor = vec4(intensity, 1.0)*texVec;
}


