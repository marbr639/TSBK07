#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 exTexCoord;
out vec3 exNormal;
out vec3 exPosition;
out vec3 groundPosition;
out vec3 groundNormal;

// NY
uniform mat4 projMatrix;
uniform mat4 modelWorld;
uniform mat4 worldView;

void main(void)
{
    mat4 mdlMatrix = worldView*modelWorld;
	mat3 normalMatrix1 = mat3(mdlMatrix);
    groundPosition = inPosition;
    groundNormal = inNormal;
    exNormal = normalMatrix1 * inNormal; // Phong
    exPosition = vec3(mdlMatrix*vec4(inPosition,1.0)); // For interpolation of vertex positions
	exTexCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
