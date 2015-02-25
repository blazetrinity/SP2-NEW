#ifndef INTERACTION_H
#define INTERACTION_H

#include "Bound.h"
#include "Vector3.h"

class CInteraction
{
public:
	void setInteractionBound(Vector3 Min, Vector3 Max);
	Vector3 getMaxBound();
	Vector3 getMinBound();
	void setLevel(int lvl);
	int getLevel();

private:
	CBound interactionBound;
	int level;
};

#endif