#version 150


out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exPosition;
in vec2 exTexCoord;
in vec3 groundPosition;
in vec3 groundNormal;

uniform vec3 lightSourcesDirPosArr[4];

uniform vec3 lightSourcesColorArr[4];

uniform bool isDirectional[4];

uniform bool isObject;

uniform mat4 worldView;

uniform sampler2D tex1;
uniform sampler2D tex2;
void main(void)
{
    vec3 light, iDiff, gPosition;
    vec3 normal = vec3(0);
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
    
    vec3 xpos = vec3(1,0,0);
    vec3 zpos = vec3(0,0,1);
    vec3 ypos = vec3(0,1,0);

    
    if (isObject)
    {
         texVec = texture(tex2, 10*exTexCoord);
         outColor = vec4(intensity, 1.0)*texVec;
    }
    else if ((dot(ypos, groundPosition) < 0.6) && ((dot(xpos, groundPosition) < 150)) && (dot(zpos, groundPosition) < 150))
        {
        outColor = vec4(0.3,0.46,1.0,0.0);
        }
    else
        {
         texVec = (dot(ypos, groundPosition)/8)*texture(tex1, 10*exTexCoord) + ((8 - dot(ypos, groundPosition))/8)*texture(tex2, 10*exTexCoord);
         outColor = vec4(intensity, 1.0)*texVec;
        }

        
}


