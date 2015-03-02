/******************************************************************************/
/*!
\file	Utility.cpp
\author Tang Wen Sheng
\par	email: tag_wen_sheng\@nyp.edu.sg
\brief
Define the Utility and it's method
*/
/******************************************************************************/

#include "Utility.h"

/***********************************************************/
/*!
\brief
	position of the Utility

\param lhs
	lhs of the Mtx44

\param rhs
	rhs of the Mtx44
*/
/***********************************************************/
Position operator*(const Mtx44& lhs, const Position& rhs)
{
	float b[4];
	for(int i = 0; i < 4; i++)
	{
		b[i] = lhs.a[0 * 4 + i] * rhs.x 
			+ lhs.a[1 * 4 + i] * rhs.y 
			+ lhs.a[2 * 4 + i] * rhs.z 
			+ lhs.a[3 * 4 + i] * 1;
	}

	return Position(b[0], b[1], b[2]);
}