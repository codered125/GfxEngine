#include "glut.h"
#include "stdafx.h"
//#include <iostream>
//#include <sstream>
//#include <vector>

//#pragma once

class Point
{
private:

	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Point(); //Default consstructer 
	Point(GLfloat a, GLfloat b, GLfloat c);
	//--------------------------------------------------------------------------------Getters--------------------------------------------------------------//
	__forceinline GLfloat getX() { return x; };
	__forceinline GLfloat getY() { return y; };
	__forceinline GLfloat getZ() { return z; };
	//---------------------------------------------------------------------------------Setters-------------------------------------------------------------//
	void setLocation(GLfloat input1, GLfloat input2, GLfloat input3);


};

class Face {
public:
	Face();
	enum  FaceType { None, Front, Back, Left, Right, Top, Bottom };
	FaceType thisFaceType = FaceType::None;

	Point generalCenterPoint, ourCenterPoint;

	Point  Points[4];

	GLfloat width = 1.0f;
	GLfloat	height = 1;


	void DetermineFaceType();

	void DrawFrontAndBack();

	void DrawLeftAndRight();

	void DrawTopAndBottom();


};

class Shape
{
public:
	Shape();
	Shape(Point *  p1, Point * p2, Point * p3, Point * p4);

	int VerticeCount;
	//Shape(Point p1, Point p2, Point p3, Point p4);
	//Shape(Point p1, Point p2)
private:
	Point * Vert1;
	Point * Vert2;
	Point * Vert3;
	Point * Vert4;
	Point * Vert5;

public:
	Point *Array[4];
	void SetPoints(Point * input1, Point * input2, Point * input3, Point * input4);
	void SetColourRGBA(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
	void SetColourRGB(GLfloat R, GLfloat G, GLfloat B);
	void SetRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void SetWorldScale(GLfloat X, GLfloat Y, GLfloat Z);
	void Translate(Shape* target, float value, int TranslationAxis);
	virtual void SetVerticeCount(int input);
	void Draw();// GLuint texID);

	__forceinline  Point GetPoint1() { return *Vert1; };
	__forceinline  Point GetPoint2() { return *Vert2; };
	__forceinline  Point GetPoint3() { return *Vert3; };
	__forceinline  Point GetPoint4() { return *Vert4; };

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

	void setPoints();
	void DetermineFaces(Point inputCenter);
	void Draw();// GLuint texID);
	void SetFaces(Face & inFront, Face & inBack, Face & inLeft, Face & inRight, Face & inTop, Face & inBottom );
};
