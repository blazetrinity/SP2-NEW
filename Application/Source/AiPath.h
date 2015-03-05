/***********************************************************/
/*!
\file	AiPath.h
\author	Malcolm Lim
\par
\brief	Creates an AI Path and storing the Information
*/
/***********************************************************/

#ifndef AIPATH_H
#define AIPATH_H
#include <vector>
#include "Vector3.h"

using std::vector;


/***********************************************************/
/*!
	class CAiPath
\brief	Defines an AI Path and it's method
*/
/***********************************************************/
class CAiPath
{
public:
	CAiPath();
	~CAiPath();
	void AddPoint(Vector3 Point);
	Vector3 getCurrentPoint();
	void CalNextPoint();
	Vector3 getNextPoint();

private:
	vector<Vector3> Points;
	Vector3 CurrentPoint;
	int numPoints;
	int currentPointIndex;
};
#endif