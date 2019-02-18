// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include <math.h>
#include "VectorUtils3.h"

// Globals
// Data would normally be read from files

GLfloat pi = 3.14159265359;
GLfloat t=0;
GLfloat r = 0;
GLuint groundTex, boxTex;

GLfloat angle = 0;

mat4 trans;
//uniform vec3 bladeColor;

mat4 projectionMatrix, modelToWorld, worldToView, modelViewMatrix;
Model *ground, *walls, *blade, *balcony, *roof, *skybox;
GLuint program, ground_shaders, skybox_shaders;

mat3 rotationy;
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
void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}





void init(void)
{
	// vertex buffer object, used for uploading the geometry

	// Reference to shader program
//	GLuint program;
	dumpInfo();

    projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 30.0);
	// GL inits
	glClearColor(0.2,10,0.5,0.5);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
    ground_shaders = loadShaders("ground3-4.vert", "ground3-4.frag");
    skybox_shaders = loadShaders("skybox3-3.vert", "skybox3-3.frag");
	printError("init shader");
	    
    ground = LoadModelPlus("ground.obj");
    walls = LoadModelPlus("windmill-walls.obj");
    blade = LoadModelPlus("blade.obj");
    roof = LoadModelPlus("windmill-roof.obj");
    balcony = LoadModelPlus("windmill-balcony.obj");
    skybox = LoadModelPlus("skybox.obj");


// Ny kod sen det funkade senaste
    glUseProgram(ground_shaders);
    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("grass.tga", &groundTex);
    glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniform1i(glGetUniformLocation(ground_shaders, "texUnit"), 0); 


    glUseProgram(skybox_shaders);
    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("SkyBox512.tga", &boxTex);
    glBindTexture(GL_TEXTURE_2D, boxTex);
	glUniform1i(glGetUniformLocation(skybox_shaders, "texUnit"), 0); 

    glUseProgram(program);

    
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
        //glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    glUseProgram(ground_shaders);

    
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

//slut på tillagd kod
	 
	printError("init arrays");
}


GLfloat stepx;
GLfloat stepz = 5;
GLfloat viewz = 0;
GLfloat viewx;
void look2(int x, int y)
{

    vec3 pos = {stepx,0,stepz};
    if (x < glutGet(GLUT_WINDOW_WIDTH)/2)
       {
        r -= 0.015;
        stepx = 5*sin(r);
        stepz = 5*cos(r);
        }
   
    if ( x > glutGet(GLUT_WINDOW_WIDTH)/2)
    {
         r += 0.015;
        stepx = 5*sin(r);
        stepz = 5*cos(r);
         
         
    }

    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_HEIGHT)/2);
}

void look(int x, int y)
{

    vec3 pos = {stepx,0,stepz};
    if (x < glutGet(GLUT_WINDOW_WIDTH)/2)
       {
        r -= 0.015;
        viewx = viewx - Norm(pos)*r*cos(r);
        viewz = viewz - Norm(pos)*r*sin(r);
        }
   
    if ( x > glutGet(GLUT_WINDOW_WIDTH)/2)
    {
         r += 0.015;
        viewx = viewx + Norm(pos)*r*cos(r);
        viewz = viewz + Norm(pos)*r*sin(r);
         
         
    }

    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_HEIGHT)/2);
}

void keyboard(char key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        stepx -= 1;
        viewx = stepx;
    if (key == GLUT_KEY_RIGHT)
        stepx += 1;
        viewx = stepx;;
    if (key == GLUT_KEY_DOWN)
    {
        stepz += 1;
        viewz += 1;
    }
    if (key == GLUT_KEY_UP)
    {
        stepz -= 1;
        viewz -= 1; 
    }
    if (key == GLUT_KEY_LEFT_SHIFT)
{
        r += 0.025;
    viewx = viewx + r*cos(r);
    viewz = viewz + r*sin(r);
        }
    if (key == GLUT_KEY_RIGHT_SHIFT)
{
    r -= 0.025;
    viewx = viewx - r*cos(r);
    viewz = viewz - r*sin(r);
 }        
}

void look1(int x, int y)
{
    if (x < glutGet(GLUT_WINDOW_WIDTH)/2)
    {
         r += 0.08;
    }
    else if ( x > glutGet(GLUT_WINDOW_WIDTH)/2)
    {
        r -= 0.08;
    }
    
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    angle+=0.01f;
            
 
    mat4 d, worldToView, modelToWorld;

    
    worldToView = lookAt(stepx,1,stepz,viewx,1,viewz,0,1,0);
    //worldToView = lookAt(5*sin(r),0.92,5*cos(r),0,0,0,0,1,0);

    glDisable(GL_DEPTH_TEST);
    modelToWorld= IdentityMatrix();
    glUseProgram(skybox_shaders);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shaders, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shaders, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shaders, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1i(glGetUniformLocation(skybox_shaders, "texUnit"), 0);
    glBindTexture(GL_TEXTURE_2D, boxTex);
    DrawModel(skybox, skybox_shaders, "inPosition", NULL, "inTexCoord");

    

    glEnable(GL_DEPTH_TEST);
    modelToWorld = Mult(T(0,0,0), S(100,100,100));
    glUseProgram(ground_shaders);
    glUniformMatrix4fv(glGetUniformLocation(ground_shaders, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(ground_shaders, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(ground_shaders, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1i(glGetUniformLocation(ground_shaders, "texUnit"), 0);
    glBindTexture(GL_TEXTURE_2D, groundTex);
    DrawModel(ground, ground_shaders, "inPosition", "inNormal", "inTexCoord");


    glUseProgram(program);

    modelToWorld = Mult(T(0,0,0), S(0.1,0.1,0.1));
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[3]);
    glUniform3f(glGetUniformLocation(program, "texVec"), 0.9,0.9,0.9);
    DrawModel(walls, program, "inPosition", "inNormal", NULL);

    glUniform3f(glGetUniformLocation(program, "texVec"), 0.0,0.0,0.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
    DrawModel(roof, program, "inPosition", "inNormal", NULL);


    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[3]);
    DrawModel(balcony, program, "inPosition", "inNormal", NULL);
 
    glUniform3f(glGetUniformLocation(program, "texVec"), 0.5,0.35,0.05);
    modelToWorld =Mult(T(0.45,0.92,0), Mult(Rx(angle), S(0.07,0.07,0.07)));  
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[4]);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);


    modelToWorld = Mult(T(0.45,0.92,0), Mult(Rx((pi/2)+angle), S(0.07,0.07,0.07)));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[4]);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);


    modelToWorld = Mult(T(0.45,0.92,0), Mult(Rx(pi + angle), S(0.07,0.07,0.07)));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[4]);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);


    modelToWorld = Mult(T(0.45,0.92,0), Mult(Rx( (3*pi/2) + angle), S(0.07,0.07,0.07)));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE,   modelToWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE,   worldToView.m);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[4]);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);

                
	printError("display");
	
	glutSwapBuffers();
}





int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
    OnTimer(0);
    glutPassiveMotionFunc(look2);
    glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
