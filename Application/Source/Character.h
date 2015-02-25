#ifndef CHARACTER_H
#define CHARACTER_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Application.h"
#include "SceneObj.h"
#include "Camera3.h"
#include <vector>

using std::vector;

class Ccharacter
{
public:
	Ccharacter();
	~Ccharacter();
	
	int getLevel();
	
	float GetRotation();

	void Update(double dt,vector<CSceneObj> Objs);
	void setLevel(int lvl);
	
	void setModel(CModel::GEOMETRY_TYPE model, CModel::GEOMETRY_TYPE modelArm);
	void setModelPosition(float x, float y, float z);

	void AddToInventory(CModel::GEOMETRY_TYPE ShelfObject);

	CModel::GEOMETRY_TYPE GetModel();
	CModel::GEOMETRY_TYPE GetModelArm();
	
	Vector3 GetPosition();
	Camera3 GetCamera();
	
	bool BoundChecking(vector<CSceneObj> Objs);

	vector<CModel::GEOMETRY_TYPE> GetInterventory();
	
private:
	bool updatePosition;
	Vector3 Position;
	Vector3 Displacement;
	Vector3 tempPosition;
	Mtx44 Rotation;
	Camera3 firstpersoncamera;
	
	CModel model;
	CModel modelArm;

	float turnbody;
	float movebody;

	int level;

	int inventorySize;
	int maxInventorySize;

	vector<CModel::GEOMETRY_TYPE> Inventory;
};

#endif