#include"AiPath.h"

CAiPath::CAiPath()
{
	numPoints = 0;
	currentPointIndex = 0;
}

CAiPath::~CAiPath()
{

}

void CAiPath::AddPoint(Vector3 Point)
{
	Points.push_back(Point);
	numPoints++;
	CurrentPoint = Points[0];
}

Vector3 CAiPath::getCurrentPoint()
{
	return CurrentPoint;
}

void CAiPath::CalNextPoint()
{
	currentPointIndex++;
	CurrentPoint = Points[(currentPointIndex % numPoints)];
	if(currentPointIndex == numPoints)
	{
		currentPointIndex = 0;
	}
}

Vector3 CAiPath::getNextPoint()
{
	return Points[((currentPointIndex + 1 )% numPoints)];
}