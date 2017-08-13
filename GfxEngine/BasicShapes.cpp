#include "ShaderCompile.h"
#include "BasicShapes.h"
Point::Point()
{
}

Point::Point(GLfloat a, GLfloat b, GLfloat c)
{
	xyz[0] = a;
	xyz[1] = b;
	xyz[2] = c;
}
//---------------------------------------------------------------------------------Setters-------------------------------------------------------------//
void Point::setLocation(GLfloat input1, GLfloat input2, GLfloat input3)
{
	xyz[0] = input1;
	xyz[1] = input2;
	xyz[2] = input3;
}



Face::Face()
{
}

void Face::DetermineCentrerPoint()
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
	//ourCenterPoint = generalCenterPoint;
}

void Cube::setPoints(Point inputCenter)
{
	ArrayHolder[0] = Front;
	Front->thisFaceType = Face::FaceType::Front;
	Front->normal.setLocation(0.0f, 0.0f, 1.0f);
	Front->generalCenterPoint = inputCenter;
	
	ArrayHolder[1] = Back;
	Back->thisFaceType = Face::FaceType::Back;
	Back->normal.setLocation(0.0f, 0.0f, -1.0f);
	Back->generalCenterPoint = inputCenter;

	ArrayHolder[2] = Left;
	Left->thisFaceType = Face::FaceType::Left;
	Left->normal.setLocation(-1.0f, 0.0f, 0.0f);
	Left->generalCenterPoint = inputCenter;

	ArrayHolder[3] = Right;
	Right->thisFaceType = Face::FaceType::Right;
	Right->normal.setLocation(1.0f, 0.0f, 0.0f);
	Right->generalCenterPoint = inputCenter;

	ArrayHolder[4] = Top;
	Top->thisFaceType = Face::FaceType::Top;
	Top->normal.setLocation(0.0f, 1.0f, 0.0f);
	Top->generalCenterPoint = inputCenter;

	ArrayHolder[5] = Bottom;
	Bottom->thisFaceType = Face::FaceType::Bottom;
	Bottom->normal.setLocation(0.0f, -1.0f, 0.0f);
	Bottom->generalCenterPoint = inputCenter;
}


void Cube::Draw()
{
	int pointCounter = 0;

   glBegin(GL_QUADS);
	for (int FaceIterator = 0; FaceIterator < 6; FaceIterator++) {	
		glNormal3f(ArrayHolder[FaceIterator]->normal.getX(), ArrayHolder[FaceIterator]->normal.getY(), ArrayHolder[FaceIterator]->normal.getZ());
		ArrayHolder[FaceIterator]->DetermineCentrerPoint();
		for (int PointIterator = 0; PointIterator < 4; PointIterator++)
		{
			 holder1[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getX();
			 holder2[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getY();
			 holder3[pointCounter] = ArrayHolder[FaceIterator]->Points[PointIterator].getZ();
			 holderEmbed[pointCounter][0] = holder1[pointCounter];
			 holderEmbed[pointCounter][1] = holder2[pointCounter];
			 holderEmbed[pointCounter][2] = holder3[pointCounter];
			 holderEmbed[pointCounter][3] = 1.0f;
			 glVertex4f(holder1[pointCounter] + 2.0f, holder2[pointCounter] + 2.0f, holder3[pointCounter] + 1.0f, 1.0f);
			 pointCounter++;
		}
	}
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

