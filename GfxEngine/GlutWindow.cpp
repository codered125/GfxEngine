#include "stdafx.h"
#include <Windows.h>
#include "ShaderCompile.h"
//#include <gl\GL.h>
//#include <gl\GLU.h>

//#include <iostream>
//#include <stdlib.h>
//#include "BasicShapes.h"

using namespace std;

//-------------------------Function declarations--------------------------------------------//
void idle(int value);
float angle = 0.0;
int main(int argc, char** argv);
void translate(float value, int direction);
void rotate();
void initRendering();
void handleKeypress(unsigned char key, int x, int y);
void scale();
void colourise();
void drawScene();
void handleResize(int w, int h);
void update(int value);

//--------------------------Function declarations--------------------------------------------//

Face Front, Back, Left, Right, Top, Bottom;
Point center; 
Cube cubes;

GLuint renderingProg;


int main(int argc, char** argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//Set the window size
	glutInitWindowSize(800, 800); 

	//Create the window
	glutCreateWindow("Moses' Game Engine");
	
	//Initialize rendering
	initRendering();

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0); //Add a timer

	glutMainLoop(); 
	return 0; 
}

//Initializes 3D rendering
void initRendering()
{
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE);
	

	//ShaderCode
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();


	//glCreateVertexArrays(1, &VAO);
}


//Called when the window is resized
void handleResize(int w, int h) 
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	//Switch to setting the camera perspective
	glMatrixMode(GL_PROJECTION); 

	//Set the camera perspective
	//Reset the camera
	glLoadIdentity(); 

	//The camera angle
	//The width to height ratio
	//The near Z clipping cooridante 
	//The far z clipping coordinates
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);

	
}
float _angle = 90.0f;

//Draws the 3D scene
void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat bg[]{ 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bg);
	//Switch to the drawing perspective
	glMatrixMode(GL_MODELVIEW); 

	//Reset the drawing perspective
	glLoadIdentity(); 

	glTranslatef(0.0f, 1.0f, -16.0f);

	//Ambient light
	GLfloat ambientColor[] = { 0.2f,0.5f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//positioned light
	GLfloat lightColour0[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//directional light
	GLfloat lightColour1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColour1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	
	glRotatef(_angle, 1.0f, 1.0f, 1.0f);
	
	//cubes
	center.setLocation(-1.5f, -1.0f, 1.5f);
	cubes.generalCenterPoint = center;
	cubes.SetFaces(Front, Back, Left, Right, Top, Bottom);
	cubes.setPoints(center);
	cubes.Draw();

	compileShader(cubes);

	//Send the 3D scene to the screen
	glutSwapBuffers(); 
}



void idle(int value)
{
	glutPostRedisplay();
	angle += 5;
}
//----------------------------------------------------------------------------------Calls to test the functions that we have created--------------------------------------------//
void scale()
{
	//sqrF.SetWorldScale(2.0f, 2.0f, 2.0f);
	idle(0);
}
void colourise()
{
	//sqrF.SetColourRGBA(0.0f, 1.0f, 0.0f, 0.5f);
	idle(0);
}
void rotate()
{
	//sqrF.SetRotation(angle, 1.0f, 0.0f, 0.0f);
	idle(0);
}
void translate(float value, int direction)
{
	//sqrF.Translate(&sqrF, value, direction);
	idle(0);
}


void update(int value) {
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{    
	switch (key)
	{
		case 27: //Escape key
			exit(0); //Exit the program
			break;
		case 49:// 1 Key
			rotate();
			break;

		case 50:// 2 Key
			scale();
			break;
		case 51:// 3 Key
			colourise();
			break;

		case 119:// W Key
			translate(0.3f, 2);
			break;
			
		case 115: //s Key
			translate(-0.3f, 2);
			break;

		case 97: //a key
			translate(-0.3f, 1);
			break;

		case 100://d key
			translate(0.3f, 1);
			break;
	}
}

