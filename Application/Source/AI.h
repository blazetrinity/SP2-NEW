#ifndef AI_H
#define AI_H

#include"Vector3.h"
#include"Model.h"
#include"Mtx44.h"
#include "Bound.h"

class CAi
{
public:
	enum AI_TYPE
	{
		STATIONARY = 0,
		MOVING,
		NUM_STATE,
	};

	CAi();
	~CAi();

	void Set(Vector3 NewPosition, Vector3 NewTarget, CModel::GEOMETRY_TYPE model, CModel::GEOMETRY_TYPE modelArm, AI_TYPE type, int level, int size);
	void CalMovementAndRotation();
	void CalTarget(Vector3 NewTarget);

	Vector3 getBoundMax();
	Vector3 getBoundMin();
	
	CModel::GEOMETRY_TYPE GetModel();
	CModel::GEOMETRY_TYPE GetModelArm();
	
	Vector3 GetScale();
	Vector3 GetPosition();

	Mtx44 GetRotation();

	int getLevel();

	CBound BoundCheck;

private:
	Vector3 Position;
	Vector3 Scale;
	Vector3 Displacement;
	Vector3 Target;
	Mtx44 Rotation;
	float rotatebody;
	float result;
	CModel model;
	CModel modelArm;
	AI_TYPE Type;
	int Level;
};

#endif