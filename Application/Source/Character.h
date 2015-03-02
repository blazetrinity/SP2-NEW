#ifndef CHARACTER_H
#define CHARACTER_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Application.h"
#include "SceneObj.h"
#include "Camera3.h"
#include "AI.h"
#include <vector>

using std::vector;

class Ccharacter
{
public:
	Ccharacter();
	~Ccharacter();
	
	int getLevel();
	
	float GetRotation();

	void Update(double dt,vector<CSceneObj> Objs, vector<CAi> AiList);
	void setLevel(int lvl);
	
	void setModel(CModel::GEOMETRY_TYPE model, CModel::GEOMETRY_TYPE modelArm);
	void setModelPosition(float x, float y, float z);
	void setInventorySize();
	void AddToInventory(CModel::GEOMETRY_TYPE ShelfObject);
	void Ccharacter::SetCharacterPosCamTar(Vector3 Pos, Vector3 Cam, Vector3 Tar);

	CModel::GEOMETRY_TYPE GetModel();
	CModel::GEOMETRY_TYPE GetModelArm();
	
	Vector3 GetPosition();
	Vector3 GetScale();
	Camera3 GetCamera();
	
	void BoundChecking(vector<CSceneObj> Objs, vector<CAi> AiList);

	vector<CModel::GEOMETRY_TYPE> GetInterventory();
	
private:
	bool updatePosition;
	Vector3 Position;
	Vector3 Scale;
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