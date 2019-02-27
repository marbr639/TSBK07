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

vec3 lightSourcesColorsArr[] = {{1.0f, 0.0f, 0.0f}, // Red light

                                 {0.0f, 1.0f, 0.0f}, // Green light

                                 {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f}}; // White light
GLint isDirectional[] = {0,0,1,1};
vec3 lightSourcesDirectionsPositions[] = {{10.0f, 5.0f, 0.0f}, // Red light, positional

                                      {0.0f, 5.0f, 10.0f}, // Green light, positional

                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X

                                       {0.0f, 0.0f, -1.0f} }; // White 
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
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 40.0;
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
        temp_z = -x*tex->width + tex->height;
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
        temp_z = -x*tex->width - tex->height;
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
    vec3 ind;
    
    GLint triangle = get_triangle(tex,x,z,get_quad(x,z));
    int x_floor = floor(x);
    int z_floor = floor(z);
    //int x_ceil = x_floor + 1;
    //int z_ceil = z_floor + 1;
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


//GLfloat length_to_floor_trig = sqrt(((x - x_floor)*(x - x_floor)) + ((z - z_floor)*(z - z_floor)));
//GLfloat length_to_ceil_trig = sqrt(((x - x_ceil)*(x - x_ceil)) + ((z - z_ceil)*(z - z_ceil)));

    //int ind2 = (x_ceil + z_floor * tex->width)*3 +1;
    int ind_x = (x_floor + z_floor * tex->width)*3 + ind.x;
    int ind_y = (x_floor + z_floor * tex->width)*3 + ind.y;
    int ind_z = (x_floor + z_floor * tex->width)*3 + ind.z;
    
    GLfloat n_x, n_y, n_z, p_x, p_y, p_z, height_y;
    n_x = normalArray[ind_x];
    n_y = normalArray[ind_y];
    n_z = normalArray[ind_z];
    p_x = vertexArray[ind_x];
    p_y = vertexArray[ind_y];
    p_z = vertexArray[ind_z];


    
    if (n_y == 0)
    {
        height_y = 0;
    }
    else
    {
    height_y = (((-n_x*(x - p_x) - n_z*(z - p_z))/n_y) + p_y)/40;
    }
    return height_y;
    
   // GLfloat p2 = vertexArray[ind2];
    //GLfloat p3 = vertexArray[ind3];

   /* GLfloat p1, height_x, height_z, height_tot;

    if(length_to_floor_trig < length_to_ceil_trig)
    {
    int ind1 = (x_floor + z_floor * tex->width)*3 +1;
    p1 = vertexArray[ind1];
    height_x = (x - x_floor)*(p2-p1);
    height_z = (z - z_floor)*(p3 -p1);
    }
    else
    {
    int ind1 = (x_ceil - z_ceil * tex->width)*3 +1;
    p1 = vertexArray[ind1];
    height_x = (x_ceil - x)*(p3-p1);
    height_z = (z_ceil - z)*(p2 - p1);
    }
    height_tot = p1 + height_x + height_z;

    return height_tot;
     */

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
vec3 lookAtPoint = {2, 3, 2};
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
        cam.y = cam.y + 0.1;
       lookAtPoint.y = lookAtPoint.y + 0.1;
    }
    if (key == GLUT_KEY_RIGHT_SHIFT)
 {
        cam.y = cam.y - 0.1;
        lookAtPoint.y = lookAtPoint.y - 0.1;
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

    m = LoadModelPlus("groundsphere.obj");
    
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
	
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
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);

	glUniformMatrix4fv(glGetUniformLocation(program, "modelWorld"), 1, GL_TRUE, modelView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldView"), 1, GL_TRUE, camMatrix.m);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

    angle += 0.01;
    modelView = T(5 + 5*cos(angle), height(&ttex,5 + 5*cos(angle),1), 1); 
    glUniformMatrix4fv(glGetUniformLocation(program, "modelWorld"), 1, GL_TRUE, modelView.m);
    DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
  //  printf("%f %d\n", height(&ttex,5 + 5*cos(angle),1));
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
