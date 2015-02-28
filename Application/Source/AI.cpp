#include"AI.h"
#include "MyMath.h"

CAi::CAi()
{
	rotatebody = 0;
	movebody = 0;
	angletorotate = 0;
	Scale.Set(12,12,12);
	Update = true;
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
	halfofwidth = size/2;
	Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
	Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
	BoundCheck.setBound(Min,Max);
	initAIText();
}

void CAi::Set(Vector3 NewPosition, float RotationAngle, CModel::GEOMETRY_TYPE Model, CModel::GEOMETRY_TYPE ModelArm, AI_TYPE type, int level, int size)
{
	Position = NewPosition;
	Rotation.SetToRotation(RotationAngle,0,1,0);
	angletorotate = rotatebody = RotationAngle;
	model.SetModel(Model);
	modelArm.SetModel(ModelArm);
	Type = type;
	Level = level;
	Vector3 Min, Max;
	halfofwidth = size/2;
	Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
	Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
	BoundCheck.setBound(Min,Max);
	initAIText();
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
	if(Update)
	{
		Vector3 OldVector;
		Vector3 NewVector;

		OldVector = Path.getCurrentPoint() - Path.getNextPoint();
		Path.CalNextPoint();
		NewVector = Path.getNextPoint() - Path.getCurrentPoint();
		
		OldVector.Normalize();
		NewVector.Normalize();

		result = OldVector.Dot(NewVector);

		angletorotate += (acos(result) * 180/3.14159265);
		Update = false;
	}
}
	
void CAi::UpDatePath(double dt)
{
	static const float Move = 20.f;
	static const float Turn = 25.f;

	Vector3 Distance = Path.getCurrentPoint() - Position;
	float Lenght = Distance.Length();

	bool torotate = false;

	/*if(rotatebody < angletorotate)
	{
		torotate = true;
	}*/

	if(Lenght > 1)
	{
		movebody -= (float)(Move * dt);
	}
	
	else
	{
		rotatebody += 90;
		Update = true;
	}
	
	/*if(Lenght < 1 && torotate == true)
	{
		rotatebody += (float)(Turn *dt);
	}
	
	else if(Lenght < 1 && rotatebody > angletorotate)
	{
		Update = true;
	}*/

	Displacement.Set(0,0,movebody);
	Rotation.SetToRotation(rotatebody,0,1,0);
	Displacement = Rotation * Displacement;
	Position += Displacement;

	Vector3 Min, Max;
	Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
	Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
	BoundCheck.setBound(Min,Max);

	movebody = 0;
}

void CAi::UpDateRotate(double dt)
{
	static const float Move = 20.f;
	static const float Turn = 25.f;

	if(rotatebody > angletorotate)
	{
		rotatebody -= (float)(Turn * dt);
	}

	else if(rotatebody < angletorotate)
	{
		rotatebody += (float)(Turn * dt);
	}

	Rotation.SetToRotation(rotatebody,0,1,0);

	Mtx44 AITextRotation;
	AITextRotation.SetToRotation(-rotatebody, 0, 1, 0);
	Vector3 AITextTranslation;
	AITextTranslation.Set(Position.x, Position.y + 55, Position.z);
	Vector3 AITextScale;
	AITextScale.Set(5, 5, 5);
	AIText.Set(AITextRotation, AITextTranslation, AITextScale);
}

void CAi::CalTarget(Vector3 NewTarget)
{
	Vector3 OldVector;
	Vector3 NewVector;
	Vector3 CrossProductResult;

	OldVector = Target - Position;
	NewVector = NewTarget - Position;

	OldVector.Normalize();
	NewVector.Normalize();

	CrossProductResult = OldVector.Cross(NewVector);

	result = OldVector.Dot(NewVector);

    if(CrossProductResult.y > 0)
	{
		angletorotate += (acos(result) * 180/3.14159265);
	}
	else if(CrossProductResult.y < 0)
	{
		angletorotate -= (acos(result) * 180/3.14159265);
	}

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

void CAi::AddPath(Vector3 Point)
{
	Path.AddPoint(Point);
}

CAi::AI_TYPE CAi::getAiType()
{
	return Type;
}

void CAi::SetText(){
	AIText.randomText();
}

void CAi::initAIText(){
	AIText.InitAIText();
	Mtx44 AITextRotation;
	AITextRotation.SetToRotation(-rotatebody, 0, 1, 0);
	Vector3 AITextTranslation;
	AITextTranslation.Set(Position.x, Position.y + 55, Position.z);
	Vector3 AITextScale;
	AITextScale.Set(5, 5, 5);
	AIText.Set(AITextRotation, AITextTranslation, AITextScale);
}

CAIInteraction CAi::getAIText(){
	return AIText;
}

void CAi::updateText(float updateValue)
{
	AIText.updateTime(updateValue);
}