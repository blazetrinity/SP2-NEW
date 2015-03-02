#ifndef SCENEOBJ_H
#define SCENEOBJ_H

#include "Bound.h"
#include "Model.h"
#include "Vector3.h"
#include "Mtx44.h"

class CSceneObj
{
public:
	CSceneObj(void);
	~CSceneObj(void);

	enum OBJ_TYPE
	{
		SHELF = 0,
		COUNTER,
		ATM,
		CART,
		SECURITY_CAM,
		WALL,
		DOOR,
		MONITOR,
		TYPE_TOTAL,
	};

	void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type);
	void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type, CModel::GEOMETRY_TYPE Item);
	void setModel(CModel::GEOMETRY_TYPE Model);
	void setRotate(int angle, bool x, bool y, bool z);
	void setTranslate(int x, int y, int z);
	void setScale(int x, int y, int z);
	void setBound(Vector3 Min, Vector3 Max);
	void setLevel(int lvl);
	void setOBJType(OBJ_TYPE Type);

	CModel::GEOMETRY_TYPE getModel(void);
	CModel::GEOMETRY_TYPE getItem();

	Mtx44 getRotate(void);
	
	Vector3 getTranslate(void);
	Vector3 getScale(void);
	Vector3 getBoundMin(void);
	Vector3 getBoundMax(void);
	
	int getLevel();
	
	OBJ_TYPE getOBJType();

private:
	Mtx44 Rotate;
	Vector3 Translate;
	Vector3 Scale;
	CBound boundCheck;
	CModel model;
	int Level;
	OBJ_TYPE Type;
	CModel item;
};

#endif