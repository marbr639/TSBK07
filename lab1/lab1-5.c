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
#include <math.h>

// Globals
// Data would normally be read from files
GLfloat vertices[36*3] =
{
						-0.5,-0.5,-0.5,	
						-0.5,0.5,-0.5,		
						0.5,0.5,-0.5,		
						-0.5,-0.5,-0.5,	
						0.5,0.5,-0.5,
						0.5,-0.5,-0.5,				

						0.5,0.5,-0.5,		
						-0.5,0.5,-0.5,		
						-0.5,0.5,0.5,		
						0.5,0.5,-0.5,		
						-0.5,0.5,0.5,		
						0.5,0.5,0.5,		

						-0.5,-0.5,-0.5,	
						-0.5,-0.5,0.5,		
						-0.5,0.5,0.5,		
						-0.5,-0.5,-0.5,	
						-0.5,0.5,0.5,		
						-0.5,0.5,-0.5,		

						0.5,-0.5,-0.5,		
						0.5,0.5,-0.5,		
						0.5,0.5,0.5,		
						0.5,-0.5,-0.5,		
						0.5,0.5,0.5,		
						0.5,-0.5,0.5,	

                        -0.5,-0.5,-0.5,	
						0.5,-0.5,-0.5,		
						0.5,-0.5,0.5,		
						-0.5,-0.5,-0.5,	
						0.5,-0.5,0.5,		
						-0.5,-0.5,0.5,							

						-0.5,-0.5,0.5,		
						0.5,-0.5,0.5,		
						0.5,0.5,0.5,		
						-0.5,-0.5,0.5,		
						0.5,0.5,0.5,		
						-0.5,0.5,0.5};

GLfloat color_vertices[36*3] =
{
	0.0f,0.0f,0.0f,
    0.0f,0.0f, 0.0f,
	0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f, 0.0f,
	0.0f,1.0f,0.0f,

    1.0f,0.0f,0.0f,
    1.0f,0.0f, 0.0f,
	1.0f,0.0f,0.0f,
    0.5f,0.5f,0.0f,
    0.5f,0.5f, 0.0f,
	0.5f,0.5f,0.0f,

    0.0f,0.0f,1.0f,
    0.0f,0.0f, 1.0f,
	0.0f,0.0f,1.0f,
    0.5f,0.5f,0.5f,
    0.5f,0.5f, 0.5f,
	0.5f,0.5f,0.5f,

    0.0f,0.5f,0.5f,
    0.0f,0.5f, 0.5f,
	0.0f,0.5f,0.5f,
    0.2f,0.0f,0.2f,
    0.2f,0.0f, 0.2f,
	0.2f,0.0f,0.2f,

    1.0f,1.0f,1.0f,
    1.0f,1.0f, 1.0f,
	1.0f,1.0f,1.0f,
    0.3f,0.2f,0.1f,
    0.3f,0.2f, 0.1f,
	0.3f,0.2f,0.1f,

    0.5f,1.0f,0.0f,
    0.5f,1.0f, 0.0f,
	0.5f,1.0f,0.0f,
    0.1f,0.2f,0.3f,
    0.1f,0.2f, 0.3f,
	0.1f,0.2f,0.3f};

GLfloat t=0; 

GLfloat angle = 30;

GLuint program;
// vertex array object
unsigned int vertexArrayObjID;
unsigned int colorArrayObjID;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
    unsigned int colorBufferObjID;
	// Reference to shader program
//	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2,10,0.5,0.5);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
    glGenVertexArrays(1, &colorArrayObjID);
	glBindVertexArray(colorArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &colorBufferObjID);
	
	// VBO for vertex data

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), color_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_color"));

	// End of upload of geometry

	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
    glBindVertexArray(colorArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    angle+=0.1f;
    
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
    glUniformMatrix4fv(glGetUniformLocation(program, "Ry"), 1, GL_TRUE,   Ry);
    glUniformMatrix4fv(glGetUniformLocation(program, "Rx"), 1, GL_TRUE,   Rx);
	    glUniformMatrix4fv(glGetUniformLocation(program, "Rz"), 1, GL_TRUE,   Rz);
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
