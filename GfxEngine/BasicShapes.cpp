#include "ShaderCompile.h"
#include "BasicShapes.h"
#define _SCL_SECURE_NO_WARNINGS  
#define _CRT_SECURE_NO_WARNINGS
Point::Point()
{
}

Point::Point(GLfloat a, GLfloat b, GLfloat c)
{
	x = a;
	y = b;
	z = c;
}
//---------------------------------------------------------------------------------Setters-------------------------------------------------------------//
void Point::setLocation(GLfloat input1, GLfloat input2, GLfloat input3)
{
	x = input1;
	y = input2;
	z = input3;
}

Shape::Shape()
{
}

Shape::Shape(Point * p1, Point * p2, Point * p3, Point * p4)
{
	Vert1 = p1;
	Vert2 = p2;
	Vert3 = p3;
	Vert4 = p4;
	Array[0] = p1;
	Array[1] = p2;
	Array[2] = p3;
	Array[3] = p4;

}

void Shape::SetPoints(Point *input1, Point *input2, Point *input3, Point *input4)
{
	Vert1 = input1;
	Vert2 = input2;
	Vert3 = input3;
	Vert4 = input4;
}

void Shape::SetColourRGBA(GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	glPushMatrix();
	glColor4f(R, G, B, A);
	printf("ralled");
	glPushMatrix();
}

void Shape::SetColourRGB(GLfloat R, GLfloat G, GLfloat B)
{
	glPushMatrix();
	glColor3f(R, G, B);

	glPopMatrix();
}

void Shape::SetRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	//Switch to the drawing perspective

	glLoadIdentity();
	glPushMatrix();

	glRotatef(angle, x, y, z);

	glPopMatrix();
	glutSwapBuffers();


}

void Shape::SetWorldScale(GLfloat X, GLfloat Y, GLfloat Z)
{
	glLoadIdentity();
	glPushMatrix();
	glScalef(X, Y, Z);

	glPopMatrix();
	glutSwapBuffers();

}

void Shape::Draw()//GLuint texID)
{
	glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 4; i++) {

		glVertex3f(Array[i]->getX(), Array[i]->getY(), Array[i]->getZ());

	}
	glEnd();
}

void Shape::Translate(Shape* target, float value, int TranslationAxis)
{

	if (TranslationAxis == 1)
	{
		for (int i = 0; i < VerticeCount; i++)
		{
			GLfloat tempFloat = target->Array[i]->getX();
			target->Array[i]->setLocation(tempFloat + value, target->Array[i]->getY(), target->Array[i]->getZ());

		}
	}
	else if (TranslationAxis == 2)
	{
		for (int i = 0; i < VerticeCount; i++)
		{
			GLfloat tempFloat = target->Array[i]->getY();
			target->Array[i]->setLocation(target->Array[i]->getX(), tempFloat + value, target->Array[i]->getZ());

		}
	}

	else if (TranslationAxis == 3)
	{
		for (int i = 0; i < VerticeCount; i++)
		{
			GLfloat tempFloat = target->Array[i]->getZ();
			target->Array[i]->setLocation(target->Array[i]->getX(), target->Array[i]->getY(), tempFloat + value);
		}
	}
}

void Shape::SetVerticeCount(int input)
{
	VerticeCount = input;
}

Face::Face()
{
}

void Face::DetermineFaceType()
{
	switch (thisFaceType)
	{
	default:
		break;
	case FaceType::Back:
		ourCenterPoint.setLocation(generalCenterPoint.getX(), generalCenterPoint.getY(), generalCenterPoint.getZ() + (height / 2.0f));
		DrawFrontAndBack();
		break;
	case FaceType::Front:
		ourCenterPoint.setLocation(generalCenterPoint.getX(), generalCenterPoint.getY(), generalCenterPoint.getZ() - (height / 2.0f));
		DrawFrontAndBack();
		break;
	case FaceType::Left:
		ourCenterPoint.setLocation(generalCenterPoint.getX() - (width / 2.0f), generalCenterPoint.getY(), generalCenterPoint.getZ());
		DrawLeftAndRight();
		break;
	case FaceType::Right:
		ourCenterPoint.setLocation(generalCenterPoint.getX() + (width / 2.0f), generalCenterPoint.getY(), generalCenterPoint.getZ());
		DrawLeftAndRight();
		break;
	case FaceType::Bottom:
		ourCenterPoint.setLocation(generalCenterPoint.getX(), generalCenterPoint.getY() - (height / 2.0f), generalCenterPoint.getZ());
		DrawTopAndBottom();
		break;
	case FaceType::Top:
		ourCenterPoint.setLocation(generalCenterPoint.getX(), generalCenterPoint.getY() + (height / 2.0f), generalCenterPoint.getZ());
		DrawTopAndBottom();
		break;
	}
}

void Face::DrawFrontAndBack()
{
	//Upperleft
	Points[0].setLocation(ourCenterPoint.getX() - (width / 2.0f), ourCenterPoint.getY() + (height / 2.0f), ourCenterPoint.getZ());

	//UpperRight
	Points[1].setLocation(ourCenterPoint.getX() + (width / 2.0f), ourCenterPoint.getY() + (height / 2.0f), ourCenterPoint.getZ());

	//BottomRight
	Points[2].setLocation(ourCenterPoint.getX() + (width / 2.0f), ourCenterPoint.getY() - (height / 2.0f), ourCenterPoint.getZ());

	//BottomLeft
	Points[3].setLocation(ourCenterPoint.getX() - (width / 2.0f), ourCenterPoint.getY() - (height / 2.0f), ourCenterPoint.getZ());



}

void Face::DrawLeftAndRight()
{
	//FrontTop
	Points[0].setLocation(ourCenterPoint.getX(), ourCenterPoint.getY() + (height / 2.0f), ourCenterPoint.getZ() - (width / 2.0f));

	//BackTop
	Points[1].setLocation(ourCenterPoint.getX(), ourCenterPoint.getY() + (height / 2.0f), ourCenterPoint.getZ() + (width / 2.0f));

	//BackBottom
	Points[2].setLocation(ourCenterPoint.getX(), ourCenterPoint.getY() - (height / 2.0f), ourCenterPoint.getZ() + (width / 2.0f));

	//FrontBottom
	Points[3].setLocation(ourCenterPoint.getX(), ourCenterPoint.getY() - (height / 2.0f), ourCenterPoint.getZ() - (width / 2.0f));
}

void Face::DrawTopAndBottom()
{
	//FrontTop
	Points[0].setLocation(ourCenterPoint.getX() - (width / 2.0f), ourCenterPoint.getY(), ourCenterPoint.getZ() - (width / 2.0f));

	//BackTop
	Points[1].setLocation(ourCenterPoint.getX() - (width / 2.0f), ourCenterPoint.getY(), ourCenterPoint.getZ() + (width / 2.0f));

	//BackBottom
	Points[2].setLocation(ourCenterPoint.getX() + (width / 2.0f), ourCenterPoint.getY(), ourCenterPoint.getZ() + (width / 2.0f));

	//FrontBottom
	Points[3].setLocation(ourCenterPoint.getX() + (width / 2.0f), ourCenterPoint.getY(), ourCenterPoint.getZ() - (width / 2.0f));
}


Cube::Cube()
{
}

Cube::Cube(Point generalCenterPoint)
{

}

void Cube::setPoints()
{
	ArrayHolder[0] = Front;
	Front->thisFaceType = Face::FaceType::Front;
	Front->normal.setLocation(0.0f, 0.0f, 1.0f);
	
	ArrayHolder[1] = Back;
	Back->thisFaceType = Face::FaceType::Back;
	Back->normal.setLocation(0.0f, 0.0f, -1.0f);
	
	ArrayHolder[2] = Left;
	Left->thisFaceType = Face::FaceType::Left;
	Left->normal.setLocation(-1.0f, 0.0f, 0.0f);
	
	ArrayHolder[3] = Right;
	Right->thisFaceType = Face::FaceType::Right;
	Right->normal.setLocation(1.0f, 0.0f, 0.0f);
	
	ArrayHolder[4] = Top;
	Top->thisFaceType = Face::FaceType::Top;
	Top->normal.setLocation(0.0f, 1.0f, 0.0f);

	ArrayHolder[5] = Bottom;
	Bottom->thisFaceType = Face::FaceType::Bottom;
	Bottom->normal.setLocation(0.0f, -1.0f, 0.0f);
}

void Cube::DetermineFaces(Point inputCenter)
{
	for (Face* iterator : ArrayHolder)
	{
		iterator->generalCenterPoint = inputCenter;
	}
}

void Cube::Draw()
{
	int pointCounter = 0;
	//GLfloat holder1[24][4];
	GLfloat holder1[24];
	GLfloat holder2[24];
	GLfloat holder3[24];
	glBegin(GL_QUADS);
	for (int FaceIterator = 0; FaceIterator < 6; FaceIterator++) {	
		glNormal3f(ArrayHolder[FaceIterator]->normal.getX(), ArrayHolder[FaceIterator]->normal.getY(), ArrayHolder[FaceIterator]->normal.getZ());
		ArrayHolder[FaceIterator]->DetermineFaceType();
		for (int PointIterator = 0; PointIterator < 4; PointIterator++)
		{
			 holder1[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getX();
			 holder2[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getY();
			 holder3[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getZ();
			glVertex3f(holder1[pointCounter], holder2[pointCounter], holder3[pointCounter]);
			pointCounter++;
		}
	}
	glVertexAttrib1fv(0, holder1);
	glVertexAttrib1fv(1, holder2);
	glVertexAttrib1fv(2, holder3);
	glDrawArrays(GL_QUADS, 0, 24);
	glEnd();
	
}

void Cube::SetFaces(Face &inFront, Face  &inBack, Face  &inLeft, Face & inRight, Face & inTop, Face & inBottom)
{
	Front = &inFront;
	Back = &inBack;
	Left = &inLeft;
	Right = &inRight;
	Top = &inTop;
	Bottom = &inBottom;
}

