#include "glut.h"
#include "include\GLFW\glfw3.h"
#include "stdafx.h"
//#include <iostream>
//#include <sstream>
//#include <vector>

//#pragma once

class Point
{
private:

	GLfloat xyz [3] ;
public:
	Point(); //Default consstructer 
	Point(GLfloat a, GLfloat b, GLfloat c);
	//--------------------------------------------------------------------------------Getters--------------------------------------------------------------//
	__forceinline GLfloat getX() { return xyz[0]; };
	__forceinline GLfloat getY() { return xyz[1]; };
	__forceinline GLfloat getZ() { return xyz[2]; };
	//---------------------------------------------------------------------------------Setters-------------------------------------------------------------//
	void setLocation(GLfloat input1, GLfloat input2, GLfloat input3);


};

class Face {
public:
	Face();
	enum  FaceType { None, Front, Back, Left, Right, Top, Bottom };
	FaceType thisFaceType = FaceType::None;

	Point generalCenterPoint, ourCenterPoint, normal;

	Point  Points[4];

	GLfloat width = 1.0f;
	GLfloat	height = 1;


	void DetermineCentrerPoint();

	void DrawFrontAndBack();

	void DrawLeftAndRight();

	void DrawTopAndBottom();

	};

class Cube
{
public:
	Cube();
	Cube(Point generalCenterPoint);

private:
	Face* Front;
	Face* Back;
	Face* Left;
	Face* Right;
	Face* Top;
	Face* Bottom;

public:
	Face* ArrayHolder[6];
	Point generalCenterPoint;

	GLfloat holder1[24];
	GLfloat holder2[24];
	GLfloat holder3[24];
	GLfloat holderEmbed[24][4];
	void setPoints(Point inputCenter);
	void Draw();// GLuint texID);
	void SetFaces(Face & inFront, Face & inBack, Face & inLeft, Face & inRight, Face & inTop, Face & inBottom );
};
