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

GLfloat t=0; 

mat4 rot, trans, total;


#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5




GLfloat angle = 30;
GLuint myTex;
GLuint mynewTex;


Model *m;
Model *c;
GLuint program;
// vertex array object
unsigned int bunnyVertexArrayObjID;



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

	// GL inits
	glClearColor(0.2,10,0.5,0.5);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");
	
    m = LoadModelPlus("bunnyplus.obj");
    c = LoadModelPlus("bilskiss.obj");

	// Upload geometry to the GPU:
	LoadTGATextureSimple("maskros512.tga", &myTex);

    glBindTexture(GL_TEXTURE_2D, myTex);

	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 

	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    angle+=0.01f;
 

    GLfloat Ry[] = {cos(angle),-0.0f, sin(angle), 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                      - sin(angle), 0.0f, cos(angle), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

GLfloat Rx[] = {1.0f,-0.0f, 0, 0.0f,
                     0.0f, cos(angle), -sin(angle), 0.0f,
                      0.0f, sin(angle), cos(angle), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

GLfloat Rz[] = {cos(angle),-sin(angle), 0.0f, 0.0f,
                     sin(angle), cos(angle), 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };

GLfloat transm[] = {1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, -3.0f,
                    0.0f, 0.0f, 0.0f, 1.0f};

GLfloat transc[] = {1.0f, 0.0f, 0.0f, 3.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, -2.0f,
                    0.0f, 0.0f, 0.0f, 1.0f};

    mat4 camMatrix = lookAt(5*sin(t*0.001),0,-3 +(5*cos(t*0.001)),0,0,-3,0,1,0);
    


    glUniformMatrix4fv(glGetUniformLocation(program, "Ry"), 1, GL_TRUE,   Ry);
    glUniformMatrix4fv(glGetUniformLocation(program, "Rx"), 1, GL_TRUE,   Rx);
	    glUniformMatrix4fv(glGetUniformLocation(program, "Rz"), 1, GL_TRUE,   Rz);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "trans"), 1, GL_TRUE,   transm);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE,   camMatrix.m);

    DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");


    glUniformMatrix4fv(glGetUniformLocation(program, "Ry"), 1, GL_TRUE,   Ry);
    glUniformMatrix4fv(glGetUniformLocation(program, "Rx"), 1, GL_TRUE,   Rx);
	    glUniformMatrix4fv(glGetUniformLocation(program, "Rz"), 1, GL_TRUE,   Rz);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE,   projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "trans"), 1, GL_TRUE,   transc);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE,   camMatrix.m);
    
    DrawModel(c, program, "in_Position", "in_Normal", "inTexCoord");
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
