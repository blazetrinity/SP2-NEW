#ifndef AI_H
#define AI_H

#include"Vector3.h"
#include"Model.h"
#include"Mtx44.h"
#include "Bound.h"
#include "AiPath.h"
#include "MyMath.h"
#include "AIInteractions.h"

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
	void Set(Vector3 NewPosition, float RotationAngle, CModel::GEOMETRY_TYPE Model, CModel::GEOMETRY_TYPE ModelArm, AI_TYPE type, int level, int size);
	void AddPath(Vector3 Point);
	void CalMovementAndRotation();
	void CalTarget(Vector3 NewTarget);
	void UpDatePath(double dt, Vector3 CharacterMin, Vector3 CharacterMax, int CharacterLevel);
	void UpDateRotate(double dt);
	void UpDateAI(double dt);
	void SetText();
	void initAIText();
	void updateText(float updateValue);
	CAIInteraction getAIText();

	AI_TYPE getAiType();

	Vector3 getBoundMax();
	Vector3 getBoundMin();
	
	CModel::GEOMETRY_TYPE GetModel();
	CModel::GEOMETRY_TYPE GetModelArm();
	
	Vector3 GetScale();
	Vector3 GetPosition();

	Mtx44 GetRotation();

	int getLevel();

	CBound BoundCheck;

	CAiPath getPath();

private:
	Vector3 Position;
	Vector3 Scale;
	Vector3 Displacement;
	Vector3 Target;
	Mtx44 Rotation;
	float movebody;
	float rotatebody;
	float result;
	float angletorotate;
	float rotatedangle;
	CModel model;
	CModel modelArm;
	AI_TYPE Type;
	CAiPath Path;
	int Level;
	bool Update;
	bool Torotate;
	float halfofwidth;
	CAIInteraction AIText;
};

#endif