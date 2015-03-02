/******************************************************************************/
/*!
\file	Interaction.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Character
*/
/******************************************************************************/

#include "Interaction.h"

/***********************************************************/
/*!
\brief
	Sets the Interaction Bound

\param Min
	Sets the minimum bound 

\param Max
	Sets the maximum bound
*/
/***********************************************************/
void CInteraction::setInteractionBound(Vector3 Min, Vector3 Max)
{
	interactionBound.setBound(Min,Max);
}
	
/***********************************************************/
/*!
\brief
	Gets the Maximum Bound

\return
	Returns the Maximum Bound
*/
/***********************************************************/
Vector3 CInteraction::getMaxBound()
{
	return interactionBound.getBoundMax();
}
	
/***********************************************************/
/*!
\brief
	Gets the Minimum Bound

\return	
	Gets the Minimum Bound
*/
/***********************************************************/
Vector3 CInteraction::getMinBound()
{
	return interactionBound.getBoundMin();
}

/***********************************************************/
/*!
\brief
	Sets the Level 
	
\param lvl
	Sets the Level
*/
/***********************************************************/
void CInteraction::setLevel(int lvl)
{
	level = lvl;
}
	
/***********************************************************/
/*!
\brief
	Gets the Level

\return
	Returns the Level
*/
/***********************************************************/
int CInteraction::getLevel()
{
	return level;
}