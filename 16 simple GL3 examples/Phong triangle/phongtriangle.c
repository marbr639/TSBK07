// Phong shaded triangle, diffuse light only

// gcc phongtriangle.c ../../common/*.c ../../common/Linux/*.c -lGL -o phongtriangle -I../../common -I../../common/Linux -DGL_GLEXT_PROTOTYPES  -lXt -lX11 -lm

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

// Globals
// Data would normally be read from files
GLfloat vertices[] = {	-0.5f,-0.5f,0.0f,
						-0.5f,0.5f,0.0f,
						0.5f,-0.5f,0.0f };

// NEW
GLfloat normals[] = {	-0.7f, 0.0f, 0.7f,
						0.0f, 1.0f, 0.0f,
						0.58f, -0.58f, 0.58f };

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int normalsBufferObjID;
	// Reference to shader program
	GLuint program;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("phong.vert", "phong.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &normalsBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	// VBO for normals data
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
	
	// End of upload of geometry
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 Phong shading example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}

