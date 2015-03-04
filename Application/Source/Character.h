/***********************************************************/
/*!
\file	Character.h
\author	Malcolm Lim
\par
\brief	Creates a character Model and storing the Information
*/
/***********************************************************/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Application.h"
#include "SceneObj.h"
#include "Camera3.h"
#include "AI.h"

#include "SoundManager.h"

#include "Bound.h"

#include <vector>
using std::vector;

/***********************************************************/
/*!
	class Ccharacter
\brief	Defines a Character and it's method
*/
/***********************************************************/
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
	void RemoveFromInventory(int index);
	void ResetInventory();

	void SetWallet(int value);
	int GetWallet(void);

	CModel::GEOMETRY_TYPE GetModel();
	CModel::GEOMETRY_TYPE GetModelArm();
	
	Vector3 GetPosition();
	Vector3 GetScale();

	Vector3 getBoundMax();
	Vector3 getBoundMin();

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
	CSoundManager mySound;

	CBound BoundCheck;

	float halfofwidth;

	float turnbody;
	float movebody;

	float walkingTimer;

	int level;

	int inventorySize;
	int maxInventorySize;

	int wallet;

	vector<CModel::GEOMETRY_TYPE> Inventory;
};

#endif