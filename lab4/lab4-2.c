// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;
GLfloat r = 0;
GLfloat s = 0;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

GLfloat stepx;
GLfloat stepz = 5;
GLfloat viewz = 0;
GLfloat viewx;

vec3 cam = {0, 1, 8};
vec3 lookAtPoint = {2, 0, 2};
vec3 up_vec = {0,1,0};
void look(int x, int y)
{

    vec3 pos = cam;
    vec3 dirxz = {lookAtPoint.x - cam.x, lookAtPoint.y - cam.y, lookAtPoint.z - cam.z};
    vec3 diryz = {lookAtPoint.x - cam.x, 0, lookAtPoint.z - cam.z};

    vec3 cross = CrossProduct(diryz, up_vec);

   if (x < glutGet(GLUT_WINDOW_WIDTH)/2)
       {
        r = 0.008;


        dirxz = MultVec3(Ry(r), dirxz);
        //lookAtPoint.x = lookAtPoint.x - Norm(pos)*r*cos(r);
        //lookAtPoint.z = lookAtPoint.z - Norm(pos)*r*sin(r);
        lookAtPoint.x = dirxz.x + cam.x;
        lookAtPoint.z = dirxz.z + cam.z;
        }

    if ( x > glutGet(GLUT_WINDOW_WIDTH)/2)
    {
         r = 0.008;

         dirxz = MultVec3(Ry(-r),dirxz);
        lookAtPoint.x = dirxz.x + cam.x;
        lookAtPoint.z = dirxz.z + cam.z;
        //lookAtPoint.x = lookAtPoint.x+ Norm(pos)*r*cos(r);
        //lookAtPoint.z = lookAtPoint.z + Norm(pos)*r*sin(r);
    
    }
/* if ( y > glutGet(GLUT_WINDOW_HEIGHT)/2)
    {
        s = 0.008;

        
        diryz = MultVec3(ArbRotate(cross,s),diryz);
        up_vec = CrossProduct(diryz, cross);
        lookAtPoint.x = diryz.x + cam.x;
        lookAtPoint.y = diryz.y + cam.y;
        lookAtPoint.z = diryz.z + cam.z;
    }

if ( y < glutGet(GLUT_WINDOW_HEIGHT)/2)
    {
        s = 0.008;
       
        diryz = MultVec3(ArbRotate(cross,-s),diryz);
        up_vec = CrossProduct(diryz, cross);
        lookAtPoint.x = diryz.x + cam.x;
        lookAtPoint.y = diryz.y + cam.y;
        lookAtPoint.z = diryz.z + cam.z;
        }

//	printf("%f %f\n", dirxz.x, dirxz.y );*/
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_HEIGHT)/2);
}

void keyboard(char key, int x, int y)
{
    vec3 temp = {0,1,0};
    vec3 dir = {lookAtPoint.x - cam.x, cam.y, lookAtPoint.z - cam.z} ;

    vec3 cross = CrossProduct(dir, temp);
    cross = ScalarMult(Normalize(cross), 0.1);
    cross.y = cam.y;
    if (key == GLUT_KEY_LEFT)
        {
        cam.x = cam.x - cross.x;
        cam.z = cam.z - cross.z;

        lookAtPoint.x = lookAtPoint.x - cross.x;
        lookAtPoint.z = lookAtPoint.z - cross.z;
        }
    if (key == GLUT_KEY_RIGHT)
        {

        cam.x = cam.x + cross.x;
        cam.z = cam.z + cross.z;

        lookAtPoint.x = lookAtPoint.x + cross.x;
        lookAtPoint.z = lookAtPoint.z + cross.z;
        }
    if (key == GLUT_KEY_DOWN)
    {
        dir = ScalarMult(Normalize(dir),0.1);
        dir.y = cam.y;

        cam.x = cam.x - dir.x;
        cam.z = cam.z - dir.z;
        lookAtPoint.x = lookAtPoint.x - dir.x;
        lookAtPoint.z = lookAtPoint.z - dir.z; 
    }
    if (key == GLUT_KEY_UP)
    {
        vec3 dir;
        dir.x = lookAtPoint.x - cam.x;
        dir.y = cam.y;
        dir.z = lookAtPoint.z - cam.z;
        dir = ScalarMult(Normalize(dir),0.1);
        dir.y = cam.y;

        cam.x = dir.x + cam.x;
        cam.z = dir.z + cam.z;
        lookAtPoint.x = lookAtPoint.x + dir.x;
        lookAtPoint.z = lookAtPoint.z + dir.z; 
    }
    if (key == GLUT_KEY_LEFT_SHIFT)
 {
        cam.y = cam.y + 0.1;
    }
    if (key == GLUT_KEY_RIGHT_SHIFT)
 {
        cam.y = cam.y - 0.1;
    } 

}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
    // Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 total, modelView, camMatrix;
	
	printError("pre display");

	glUseProgram(program);

	// Build matrix
	
	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);
	modelView = Mult(IdentityMatrix(), S(0.1,0.1,0.1));
	total = Mult(camMatrix, modelView);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

    glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(look);

	glutMainLoop();
	exit(0);
}
