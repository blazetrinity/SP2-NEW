#include "Interaction.h"

void CInteraction::setInteractionBound(Vector3 Min, Vector3 Max)
{
	interactionBound.setBound(Min,Max);
}
	
Vector3 CInteraction::getMaxBound()
{
	return interactionBound.getBoundMax();
}
	
Vector3 CInteraction::getMinBound()
{
	return interactionBound.getBoundMin();
}
	
void CInteraction::setLevel(int lvl)
{
	level = lvl;
}
	
int CInteraction::getLevel()
{
	return level;
}