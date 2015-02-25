#include "SceneObj.h"


CSceneObj::CSceneObj(void)
{
	Rotate.SetToIdentity();
	Translate.Set(0, 0, 0);
	Scale.Set(1, 1, 1);
}


CSceneObj::~CSceneObj(void)
{
}

void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type)
{
	model.SetModel(Model);
	Translate = translate;
	Scale = scale;
	Rotate = rotate;
	Level = level;
	boundCheck.setBound(Min, Max);
	Type = type;
}

void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type, CModel::GEOMETRY_TYPE Item)
{
	model.SetModel(Model);
	Translate = translate;
	Scale = scale;
	Rotate = rotate;
	Level = level;
	boundCheck.setBound(Min, Max);
	Type = type;
	item.SetModel(Item);
}

void CSceneObj::setModel(CModel::GEOMETRY_TYPE Model){
	model.SetModel(Model);
}

void CSceneObj::setRotate(int angle, bool x, bool y, bool z){
	Rotate.SetToRotation(angle, x, y, z);
}

void CSceneObj::setTranslate(int x, int y, int z){
	Translate.x = x;
	Translate.y = y;
	Translate.z = z;
}

void CSceneObj::setScale(int x, int y, int z){
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

void CSceneObj::setBound(Vector3 Min, Vector3 Max){
	boundCheck.setBound(Min, Max);
}

void CSceneObj::setLevel(int level)
{
	Level = level;
}

void CSceneObj::setOBJType(OBJ_TYPE type)
{
	Type = type;
}

CModel::GEOMETRY_TYPE CSceneObj::getModel(void){
	return model.getModel();
}

CModel::GEOMETRY_TYPE CSceneObj::getItem()
{
	return item.getModel();
}

Mtx44 CSceneObj::getRotate(void){
	return Rotate;
}

Vector3 CSceneObj::getTranslate(void){
	return Translate;
}

Vector3 CSceneObj::getScale(void){
	return Scale;
}

Vector3 CSceneObj::getBoundMin(void){
	return boundCheck.getBoundMin();
}

Vector3 CSceneObj::getBoundMax(void){
	return boundCheck.getBoundMax();
}

int CSceneObj::getLevel()
{
	return Level;
}

CSceneObj::OBJ_TYPE CSceneObj::getOBJType()
{
	return Type;
}
