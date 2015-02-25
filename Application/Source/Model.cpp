#include "Model.h"


CModel::CModel(void)
{
}


CModel::~CModel(void)
{
}

void CModel::SetModel(GEOMETRY_TYPE Model)
{
	model = Model;
}

CModel::GEOMETRY_TYPE CModel::getModel(){
	return model;
}