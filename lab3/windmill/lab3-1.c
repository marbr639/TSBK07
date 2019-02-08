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

GLfloat angle = 0;
GLuint myTex;
GLuint mynewTex;

mat4 projectionMatrix;
Model *blade, *walls, *roof, *balcony;
GLuint program;
// vertex array object


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
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");
	
    blade = LoadModelPlus("blade.obj");
    walls = LoadModelPlus("windmill-walls.obj");
    roof = LoadModelPlus("windmill-roof.obj");
    balcony = LoadModelPlus("windmill-balcony.obj");
	// Upload geometry to the GPU:

	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object
  //  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    angle+=0.01f;
 
mat4 worldToView, d;
    worldToView = lookAt(2,0.92,-1,0,1,0,0,1,0);
    d = Mult(worldToView, Mult(T(0,0,0), S(0.1,0.1,0.1)));

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(walls, program, "inPosition", "inNormal", NULL);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(roof, program, "inPosition", "inNormal", NULL);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(balcony, program, "inPosition", "inNormal", NULL);
 
    d = Mult(worldToView, Mult(T(0.45,0.92,0), Mult(Rx(angle), S(0.07,0.07,0.07))));
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);

    d = Mult(worldToView, Mult(T(0.45,0.92,0), Mult(Rx((pi/2)+angle), S(0.07,0.07,0.07))));
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);

    d = Mult(worldToView, Mult(T(0.45,0.92,0), Mult(Rx(pi + angle), S(0.07,0.07,0.07))));
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
    DrawModel(blade, program, "inPosition", "inNormal", NULL);

    d = Mult(worldToView, Mult(T(0.45,0.92,0), Mult(Rx( (3*pi/2) + angle), S(0.07,0.07,0.07))));
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE,   d.m);
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
    //glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
