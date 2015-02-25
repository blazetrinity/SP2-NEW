#include "Bound.h"


CBound::CBound(void)
{
}


CBound::~CBound(void)
{
}

void CBound::setBound(Vector3 Min, Vector3 Max){
	MinBound = Min;
	MaxBound = Max;
}

Vector3 CBound::getBoundMin(void){
	return MinBound;
}

Vector3 CBound::getBoundMax(void){
	return MaxBound;
}