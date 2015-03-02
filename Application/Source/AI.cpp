/******************************************************************************/
/*!
\file	AI.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Set and Render AI
*/
/******************************************************************************/

#include"AI.h"
#include "MyMath.h"

/******************************************************************************/
/*!
\brief
CAi default constructor
*/
/******************************************************************************/
CAi::CAi()
{
	rotatebody = 0;
	movebody = 0;
	angletorotate = 0;
	Scale.Set(12,12,12);
	Update = true;
}

/******************************************************************************/
/*!
\brief
CAi deconstructor
*/
/******************************************************************************/
CAi::~CAi()
{

}

/******************************************************************************/
/*!
\brief
Set the Position, Target, Model, Level and Size

\param NewPosition
	Set a new Position to the AI

\param NewTarget
	Set a new Target for the AI

\param Model
	Set a Model for the AI

\param ModelArm
	Set a ModelArm to the AI

\param Type
	Set a Type to the AI

\param level
	Set the level for the AI to render

\param size
	Set the size for the AI
*/
/******************************************************************************/
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


/******************************************************************************/
/*!
\brief
Set the Position, Target, Model, Level and Size

\param NewPosition
	Set a new Position to the AI

\param RotationAngle
	Set the rotation Angle for the AI to rotate

\param Model
	Set a Model for the AI

\param ModelArm
	Set a ModelArm to the AI

\param Type
	Set a Type to the AI

\param level
	Set the level for the AI to render

\param size
	Set the size for the AI
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
Get the models that is set

\return 
	Returns the Model
*/
/******************************************************************************/
CModel::GEOMETRY_TYPE CAi::GetModel()
{
	return model.getModel();
}
	
/******************************************************************************/
/*!
\brief
Get the models arms that is set

\return 
	Returns the Model Arms
*/
/******************************************************************************/
CModel::GEOMETRY_TYPE CAi::GetModelArm()
{
	return modelArm.getModel();
}

/******************************************************************************/
/*!
\brief
Get the Models Position

\return 
	Returns the Position
*/
/******************************************************************************/
Vector3 CAi::GetPosition()
{
	return Position;
}

/******************************************************************************/
/*!
\brief
Get the Models Scale

\return 
	Returns the Model Scale
*/
/******************************************************************************/
Vector3 CAi::GetScale()
{
	return Scale;
}

/******************************************************************************/
/*!
\brief
Calculate the Movement and the Rotation of the AI
*/
/******************************************************************************/
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
	
/******************************************************************************/
/*!
\brief
Update the path of the AI

\param dt
	Using the time to update the AI Path
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
Update the rotation of the model

\param dt
	Using the time to update the rotation value
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
Calculate the Target for the AI

\param NewTarget	
	Sets the new target for the AI
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
Get the rotation value

\return 
	Returns the rotation value
*/
/******************************************************************************/
Mtx44 CAi::GetRotation()
{
	return Rotation;
}

/******************************************************************************/
/*!
\brief
Gets the Maximum Bound for the AI

\return 
	Returns AI Maximum Bound
*/
/******************************************************************************/
Vector3 CAi::getBoundMax()
{
	return BoundCheck.getBoundMax();
}

/******************************************************************************/
/*!
\brief
Gets the Minimum Bound for the AI

\return 
	Returns AI Minimum Bound
*/
/******************************************************************************/
Vector3 CAi::getBoundMin()
{
	return BoundCheck.getBoundMin();
}

/******************************************************************************/
/*!
\brief
Gets the level for the AI to render

\return 
	Returns level for AI to render
*/
/******************************************************************************/
int CAi::getLevel()
{
	return Level;
}

/******************************************************************************/
/*!
\brief
Adds the path for the AI to move

\param Point
	Adds the point for the AI to move
*/
/******************************************************************************/
void CAi::AddPath(Vector3 Point)
{
	Path.AddPoint(Point);
}

/******************************************************************************/
/*!
\brief
Gets the AI Type 

\return 
	Returns AI Type
*/
/******************************************************************************/
CAi::AI_TYPE CAi::getAiType()
{
	return Type;
}

/******************************************************************************/
/*!
\brief
Sets the text for the AI Interaction 
*/
/******************************************************************************/
void CAi::SetText(){
	AIText.randomText();
}

/******************************************************************************/
/*!
\brief
Initialise the AI Text
*/
/******************************************************************************/
void CAi::initAIText(){
	AIText.InitAIText();
	Mtx44 AITextRotation;
	AITextRotation.SetToRotation(-rotatebody, 0, 1, 0);
	Vector3 AITextTranslation;
	AITextTranslation.Set(Position.x, Position.y + 55, Position.z);
	Vector3 AITextScale;
	AITextScale.Set(4, 4, 4);
	AIText.Set(AITextRotation, AITextTranslation, AITextScale);
}

/******************************************************************************/
/*!
\brief
Getst the AI Text 

\return 
	Returns the AI Text
*/
/******************************************************************************/
CAIInteraction CAi::getAIText(){
	return AIText;
}

/******************************************************************************/
/*!
\brief
Updates the Text time

\param updateValue
	Updates the Value for the timer to render the Text	
*/
/******************************************************************************/
void CAi::updateText(float updateValue)
{
	AIText.updateTime(updateValue);
}