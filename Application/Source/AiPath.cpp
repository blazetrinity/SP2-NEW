/******************************************************************************/
/*!
\file	AIPath.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define the AI Path
*/
/******************************************************************************/

#include"AiPath.h"

/******************************************************************************/
/*!
\brief
CAiPath default constructor
*/
/******************************************************************************/
CAiPath::CAiPath()
{
	numPoints = 0;
	currentPointIndex = 0;
}

/******************************************************************************/
/*!
\brief
CAiPath deconstructor
*/
/******************************************************************************/
CAiPath::~CAiPath()
{

}

/******************************************************************************/
/*!
\brief
Sets the AI Path

\param Point
	Sets the point for AI to move
*/
/******************************************************************************/
void CAiPath::AddPoint(Vector3 Point)
{
	Points.push_back(Point);
	numPoints++;
	CurrentPoint = Points[0];
}

/******************************************************************************/
/*!
\brief
Gets the current point the AI is in

\return
	Returns the Current point of the AI
*/
/******************************************************************************/
Vector3 CAiPath::getCurrentPoint()
{
	return CurrentPoint;
}

/******************************************************************************/
/*!
\brief
Calclate the Next Point for the AI to move

*/
/******************************************************************************/
void CAiPath::CalNextPoint()
{
	currentPointIndex++;
	CurrentPoint = Points[(currentPointIndex % numPoints)];
	if(currentPointIndex == numPoints)
	{
		currentPointIndex = 0;
	}
}

/******************************************************************************/
/*!
\brief
Get the next point for the AI to move

\return
	Returns the next point for the AI to move
*/
/******************************************************************************/
Vector3 CAiPath::getNextPoint()
{
	return Points[((currentPointIndex + 1 )% numPoints)];
}