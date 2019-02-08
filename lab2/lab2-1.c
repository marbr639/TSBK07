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

// Globals
// Data would normally be read from files

GLfloat t=0; 

GLfloat angle = 30;
GLuint tex;


Model *m;
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
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;
	// Reference to shader program
//	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2,10,0.5,0.5);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	printError("init shader");
	
    m = LoadModel("bunnyplus.obj");
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	//glGenVertexArrays(1, &vertexArrayObjID);
    glGenVertexArrays(1, &bunnyVertexArrayObjID);

	// Allocate Vertex Buffer Objects

    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glGenBuffers(1, &bunnyTexCoordBufferObjID);

    glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
	
	// VBO for vertex data
   if (m->texCoordArray != NULL)

    {

        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);

        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);

        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

    };


    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 

    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);

    // VBO for normal data

    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
	// End of upload of geometry


	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(bunnyVertexArrayObjID);
	//glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

   t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    angle+=0.01f;

GLfloat TexMatrix[] = {tan(t*0.001), 0, tan(t*0.001), 0};

GLfloat Ry[] = {cos(angle),-0.0f, sin(angle), 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                      - sin(angle), 0.0f, cos(angle), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

GLfloat Rx[] = {1.0f,-0.0f, 0, 0.0f,
                     0.0f, cos(angle), -sin(angle), 0.0f,
                      0.0f, sin(angle), cos(angle), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};

GLfloat Rz[] = {cos(angle+90),-sin(angle+90), 0.0f, 0.0f,
                     sin(angle+90), cos(angle+90), 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f};
    glUniformMatrix4fv(glGetUniformLocation(program, "Ry"), 1, GL_TRUE,   Ry);
    glUniformMatrix4fv(glGetUniformLocation(program, "Rx"), 1, GL_TRUE,   Rx);
	    glUniformMatrix4fv(glGetUniformLocation(program, "Rz"), 1, GL_TRUE,   Rz);
    glUniformMatrix2fv(glGetUniformLocation(program, "TexMatrix"), 1, GL_TRUE,   TexMatrix);
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
