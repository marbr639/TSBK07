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
GLfloat angle = 0;
TextureData ttex; // terrain
GLfloat height(TextureData*, GLfloat, GLfloat);

vec3 lightSourcesColorsArr[] = {//{1.0f, 0.0f, 0.0f}, // Red light

                                // {0.0f, 1.0f, 0.0f}, // Green light

                                // {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f}}; // White light
GLint isDirectional[] = {1};
vec3 lightSourcesDirectionsPositions[] = {{1.0f, 1.0f, 1.0f}, // Red light, positional

                                      {0.0f, 5.0f, 10.0f}, // Green light, positional

                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X

                                       {0.0f, 0.0f, -1.0f} }; // White 
GLint isObject[] = {0,1};
GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};
GLfloat *vertexArray;
GLfloat *normalArray;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 20.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
    
            if(( x > 0) && (z > 0))
            {
                int c = (x + z * tex->width)*3;
                int a = (x + z * tex->width)*3 - tex->width*3; 
                vec3 v1 = {vertexArray[c-3], vertexArray[c-2], vertexArray[c -1]};
                vec3 v2 = {vertexArray[a], vertexArray[a+1], vertexArray[a+2]};
                vec3 v3 = {vertexArray[a - 3], vertexArray[a-2], vertexArray[a-1]};
                            
                
                vec3 normal = CalcNormalVector(v1,v2,v3);
                normalArray[(x + z * tex->width)*3 + 0] = normal.x;
                normalArray[(x + z * tex->width)*3 + 1] = normal.y;
                normalArray[(x + z * tex->width)*3 + 2] = normal.z;
            }
            else
            {
			    normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			    normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			    normalArray[(x + z * tex->width)*3 + 2] = 0.0;
            }

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height;
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

// Height function


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

GLint get_quad(GLfloat x, GLfloat z)
{
    GLint quad;
    if ((x > 0) && (z > 0))
    {
        quad = 1;
    }
    else if ((x > 0) && ( z < 0))
    {
        quad = 4;
    }
    else if ((x < 0) && (z <0))
    {
        quad = 3;
    }
    else
    {
        quad = 2;
    }

    return quad;
}

GLfloat get_triangle(TextureData *tex, GLfloat x, GLfloat z, GLint quad)
{

    GLint trig;
    GLfloat temp_z;
    if (quad == 1)
    {
        temp_z = -x +1;
        if ( z > temp_z)
        {
            trig = 1;
        }
        else 
        {
            trig = 0;
        }
     }
     else if (quad == 2)
    {
       temp_z = -x;
       if ( z > temp_z)
       {
            trig = 1;
       }
        else
        {
            trig = 0;
        }
    }
    else if ( quad == 3)
    {
        temp_z = -x -1;
        if ( z > temp_z)
        {
            trig = 1;
        }
        else 
        {
            trig = 0; 
        }
     }
     else
     {
        temp_z = -x;
        if (z > temp_z)
         {
            trig = 1;
        }
        else 
        {
            trig = 0;
        }
     }

return trig;
}      
              
       
GLfloat height(TextureData *tex, GLfloat x, GLfloat z)
{
    vec3 ind, va, vb, vc, normal;
    int ind_xa, ind_ya, ind_za;
    GLint triangle = get_triangle(tex,x,z,get_quad(x,z));
    int x_floor = floor(x);
    int z_floor = floor(z);
    int x_ceil = floor(x) + 1;
    int z_ceil = floor(z) + 1;

    if (triangle == 0)
        {
            ind.x = 0;
            ind.y = 1;
            ind.z = 2;
        }
    else
        {
            ind.x = 3;
            ind.y = 4;
            ind.z = 5;
        }

    if (triangle == 0)
    {
    ind_xa = (x_floor + z_floor * tex->width)*3 + ind.x;
    ind_ya = (x_floor + z_floor * tex->width)*3 + ind.y;
    ind_za = (x_floor + z_floor * tex->width)*3 + ind.z;
    }
    else
    {
    ind_xa = (x_ceil + z_ceil * tex->width)*3 + ind.x;
    ind_ya = (x_ceil + z_ceil * tex->width)*3 + ind.y;
    ind_za = (x_ceil + z_ceil * tex->width)*3 + ind.z;
    }
    
    int ind_xb = (x_ceil + z_floor * tex->width)*3 + ind.x;
    int ind_yb = (x_ceil + z_floor * tex->width)*3 + ind.y;
    int ind_zb = (x_ceil + z_floor * tex->width)*3 + ind.z;

    int ind_xc = (x_floor + z_ceil * tex->width)*3 + ind.x;
    int ind_yc = (x_floor + z_ceil * tex->width)*3 + ind.y;
    int ind_zc = (x_floor + z_ceil * tex->width)*3 + ind.z;
    
    GLfloat height_y;

    va.x = vertexArray[ind_xa];
    va.y = vertexArray[ind_ya];
    va.z = vertexArray[ind_za];
    vb.x = vertexArray[ind_xb];
    vb.y = vertexArray[ind_yb];
    vb.z = vertexArray[ind_zb];
    vc.x = vertexArray[ind_xc];
    vc.y = vertexArray[ind_yc];
    vc.z = vertexArray[ind_zc];


    normal = CalcNormalVector(va,vb,vc);



    height_y = (((-normal.x*(x - va.x) - normal.z*(z - va.z))/normal.y) + va.y);

    return height_y;

}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;

GLfloat stepx;
GLfloat stepz = 5;
GLfloat viewz = 0;
GLfloat viewx;

vec3 cam = {0, 5, 8};
vec3 lookAtPoint = {20, 3, 2};
vec3 up_vec = {0,1,0};
void look(int x, int y)
{

    vec3 dirxz = {lookAtPoint.x - cam.x, lookAtPoint.y - cam.y, lookAtPoint.z - cam.z};

   if (x < glutGet(GLUT_WINDOW_WIDTH)/2)
       {
        r = 0.008;


        dirxz = MultVec3(Ry(r), dirxz);
        lookAtPoint.x = dirxz.x + cam.x;
        lookAtPoint.z = dirxz.z + cam.z;
        }

    if ( x > glutGet(GLUT_WINDOW_WIDTH)/2)
    {
         r = 0.008;

         dirxz = MultVec3(Ry(-r),dirxz);
        lookAtPoint.x = dirxz.x + cam.x;
        lookAtPoint.z = dirxz.z + cam.z;
    }

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
        dir = ScalarMult(Normalize(dir),0.5);
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
        dir = ScalarMult(Normalize(dir),0.5);
        dir.y = cam.y;

        cam.x = dir.x + cam.x;
        cam.z = dir.z + cam.z;
        lookAtPoint.x = lookAtPoint.x + dir.x;
        lookAtPoint.z = lookAtPoint.z + dir.z; 
    }
    if (key == GLUT_KEY_LEFT_SHIFT)
 {
        cam.y = cam.y + 1;
       lookAtPoint.y = lookAtPoint.y + 1;
    }
    if (key == GLUT_KEY_RIGHT_SHIFT)
 {
        cam.y = cam.y - 1;
        lookAtPoint.y = lookAtPoint.y - 1;
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
	program = loadShaders("terrain4-5.vert", "terrain4-5.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    m = LoadModelPlus("groundsphere.obj");
    
	glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
    LoadTGATextureSimple("dirt.tga", &tex2);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

    // Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);

     glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

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


	glUniformMatrix4fv(glGetUniformLocation(program, "modelWorld"), 1, GL_TRUE, modelView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldView"), 1, GL_TRUE, camMatrix.m);
    glUniform1f(glGetUniformLocation(program, "isObject"), isObject[0]);
//	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

    angle += 0.01;
    modelView = Mult(S(0.1,0.1,0.1),Mult(T(100 + 90*cos(angle), height(&ttex,100 + 90*cos(angle),100 + 90*sin(angle)),100+ 90*sin(angle)), S(10,10,10))); 

    glUniformMatrix4fv(glGetUniformLocation(program, "modelWorld"), 1, GL_TRUE, modelView.m);
    glUniform1f(glGetUniformLocation(program, "isObject"), isObject[1]);
    DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
//   printf("%f %d\n", height(&ttex,10 + 5*cos(angle),10 + 5*sin(angle)));
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
