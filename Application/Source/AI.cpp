#include"AI.h"
#include "MyMath.h"

CAi::CAi()
{
	rotatebody = 0;
	Scale.Set(12,12,12);
}

CAi::~CAi()
{

}

void CAi::Set(Vector3 NewPosition, Vector3 NewTarget, CModel::GEOMETRY_TYPE Model, CModel::GEOMETRY_TYPE ModelArm, AI_TYPE type, int level, int size)
{
	Position = NewPosition;
	Target.Set(Position.x,Position.y,Position.z - 20);
	CalTarget(NewTarget);
	model.SetModel(Model);
	modelArm.SetModel(ModelArm);
	Type = type;
	Level = level;
	Vector3 Min, Max;
	float halfofwidth = size/2;
	Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
	Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
	BoundCheck.setBound(Min,Max);
}

CModel::GEOMETRY_TYPE CAi::GetModel()
{
	return model.getModel();
}
	
CModel::GEOMETRY_TYPE CAi::GetModelArm()
{
	return modelArm.getModel();
}

Vector3 CAi::GetPosition()
{
	return Position;
}

Vector3 CAi::GetScale()
{
	return Scale;
}

void CAi::CalMovementAndRotation()
{

}
	
void CAi::CalTarget(Vector3 NewTarget)
{
	Vector3 OldVector;
	Vector3 NewVector;
	Vector3 CrossProductResult;

	OldVector = Target - Position;
	NewVector = NewTarget - Position;
	CrossProductResult = OldVector.Cross(NewVector);

	result = OldVector.Dot(NewVector) / (OldVector.Length() * NewVector.Length());

	if(CrossProductResult.y > 0)
	{
		rotatebody += (acos(result) * 180/3.14159265);
	}
	else if(CrossProductResult.y < 0)
	{
		rotatebody -= (acos(result) * 180/3.14159265);
	}

	Rotation.SetToRotation(rotatebody,0,1,0);

	Target = NewTarget;
}

Mtx44 CAi::GetRotation()
{
	return Rotation;
}

Vector3 CAi::getBoundMax()
{
	return BoundCheck.getBoundMax();
}

Vector3 CAi::getBoundMin()
{
	return BoundCheck.getBoundMin();
}

int CAi::getLevel()
{
	return Level;
}