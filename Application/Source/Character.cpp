/******************************************************************************/
/*!
\file	Character.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Character
*/
/******************************************************************************/

#include "Character.h"

/***********************************************************/
/*!
\brief
	Ccharacter constructor
*/
/***********************************************************/
Ccharacter::Ccharacter()
{
	Position.Set(0,-50,-100);
	Scale.Set(12,12,12);
	Displacement.Set(0,0,0);
	Rotation.SetToIdentity();
	turnbody = 0;
	movebody = 0;
	firstpersoncamera.Init(Vector3(Position.x,Position.y + 30,Position.z), Vector3(Position.x, Position.y + 30, Position.z-10), Vector3(0, 1, 0));
	level = 1;
	inventorySize = 0;
	maxInventorySize = 2;
	wallet = 1;
	halfofwidth = 7/2;

	Vector3 Min, Max;
	Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
	Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
	BoundCheck.setBound(Min,Max);
}

/***********************************************************/
/*!
\brief
	Ccharacter Destructor
*/
/***********************************************************/
Ccharacter::~Ccharacter()
{

}
	
/***********************************************************/
/*!
\brief
	Update the Character

\param dt
	Using delta time to update the Character

\param Objs
	Store the Objs in the Vector

\param AiList
	Store the AiList in a vector
*/
/***********************************************************/
void Ccharacter::Update(double dt,vector<CSceneObj> Objs, vector<CAi> AiList)
{
	static const float Move = 20.f;
	static const float Turn = 25.f;

	tempPosition = Position;
	updatePosition = false;

	if(Application::IsKeyPressed('W'))
	{
		movebody -= (float)(Move * dt);
		Displacement.Set(0,0,movebody);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);

		movebody = 0;
	}
	
	if(Application::IsKeyPressed('S'))
	{
		movebody += (float)(Move * dt);
		Displacement.Set(0,0,movebody);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);

		movebody = 0;
	}

	if(Application::IsKeyPressed('D'))
	{
		movebody += (float)(Move * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);

		movebody = 0;
	}

	if(Application::IsKeyPressed('A'))
	{
		movebody -= (float)(Move * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);

		movebody = 0;
	}

	if(Application::IsKeyPressed(VK_LEFT))
	{
		turnbody += (float)(Turn * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement =  Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);
	}

	if(Application::IsKeyPressed(VK_RIGHT))
	{
		turnbody -= (float)(Turn * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement =  Rotation * Displacement;
		tempPosition += Displacement;

		BoundChecking(Objs, AiList);
	}

	firstpersoncamera.Update(dt);

	if(updatePosition)
	{
		Position += Displacement;
		firstpersoncamera.position = Position;
		firstpersoncamera.position.y += 30;
		firstpersoncamera.target += Displacement;
		
		Vector3 Min, Max;
		Min.Set((Position.x - (halfofwidth*Scale.x)),0,(Position.z - (halfofwidth*Scale.z)));
		Max.Set((Position.x + (halfofwidth*Scale.x)),0,(Position.z + (halfofwidth*Scale.z)));
		BoundCheck.setBound(Min,Max);

		std::cout << "Character" << Position.x << " " << Position.y << " " << Position.z << std::endl;
		std::cout << "Target" << firstpersoncamera.target.x << " " << firstpersoncamera.target.y << " " << firstpersoncamera.target.z << std::endl;
	}
}

/***********************************************************/
/*!
\brief
	Gets the character's position

\return
	Returns the character's Position
*/
/***********************************************************/
Vector3 Ccharacter::GetPosition()
{
	return Position;
}

/***********************************************************/
/*!
\brief
	Gets the character's Scale

\return
	Returns the character's Scale
*/
/***********************************************************/
Vector3 Ccharacter::GetScale()
{
	return Scale;
}

/***********************************************************/
/*!
\brief
	Gets the character's Camera

\return
	Returns the character's Camera
*/
/***********************************************************/
Camera3 Ccharacter::GetCamera()
{
	return firstpersoncamera;
}

/***********************************************************/
/*!
\brief
	Gets the character's Rotation

\return
	Returns the character's Rotation
*/
/***********************************************************/
float Ccharacter::GetRotation()
{
	return turnbody;
}

void Ccharacter::SetWallet(int value)
{
	wallet += value;
}

int Ccharacter::GetWallet()
{
	return wallet;
}

/***********************************************************/
/*!
\brief
	Gets the Bound Checking for the Character

\param Objs
	Store the Objs in a Vector	

\param AiList
	Store the AI in a vector
*/
/***********************************************************/
void Ccharacter::BoundChecking(vector<CSceneObj> Objs, vector<CAi> AiList)
{	
	if(Objs.size() > 0 || AiList.size() > 0)
	{
		for(int i = 0; i < Objs.size(); ++i)
		{
			if(tempPosition.x < Objs[i].getBoundMax().x && tempPosition.x > Objs[i].getBoundMin().x && tempPosition.z < Objs[i].getBoundMax().z && tempPosition.z > Objs[i].getBoundMin().z && Objs[i].getLevel() == level)
			{
				updatePosition = false;
				return;
			}

			else if(i == (Objs.size()-1))
			{
				updatePosition = true;
			}
		}

		for(int i = 0; i < AiList.size(); ++i)
		{
			if(tempPosition.x < AiList[i].getBoundMax().x && tempPosition.x > AiList[i].getBoundMin().x && tempPosition.z < AiList[i].getBoundMax().z && tempPosition.z > AiList[i].getBoundMin().z && AiList[i].getLevel() == level)
			{
				updatePosition = false;
				return;
			}

			else if(i == (AiList.size()-1))
			{
				updatePosition = true;
			}
		}
	}

	else
	{
		updatePosition = true;
	}
}

/***********************************************************/
/*!
\brief
	Sets a model

\param setModel
	Set a model for the Character

\param SetModelArm
	Sets a model Arm for the Character
*/
/***********************************************************/
void Ccharacter::setModel(CModel::GEOMETRY_TYPE setModel, CModel::GEOMETRY_TYPE setModelArm)
{
	model.SetModel(setModel);
	modelArm.SetModel(setModelArm);
}

/***********************************************************/
/*!
\brief
	Gets a model

\return
	Returns a model 
*/
/***********************************************************/
CModel::GEOMETRY_TYPE Ccharacter::GetModel()
{
	return model.getModel();
}

/***********************************************************/
/*!
\brief
	Gets a Model Arm

\return
	Returns the model's arm
*/
/***********************************************************/
CModel::GEOMETRY_TYPE Ccharacter::GetModelArm()
{
	return modelArm.getModel();
}

/***********************************************************/
/*!
\brief
	Sets the model Position

\param defaultX
	Sets the Default X Position

\param defaultY
	Sets the Default Y Position

\param defaultZ
	Sets the Default Z Position
*/
/***********************************************************/
void Ccharacter::setModelPosition(float defaultX, float defaultY, float defaultZ)
{
	Position.Set(defaultX, defaultY, defaultZ);
}

/***********************************************************/
/*!
\brief
	Gets the Level for the player to render

\return
	Returns the level
*/
/***********************************************************/
int Ccharacter::getLevel()
{
	return level;
}

/***********************************************************/
/*!
\brief
	Sets the Level

\param lvl	
	Sets the level to render the character
*/
/***********************************************************/
void Ccharacter::setLevel(int lvl)
{
	level = lvl;
}

/***********************************************************/
/*!
\brief
	Sets the Inventory Size
*/
/***********************************************************/
void Ccharacter::setInventorySize()
{
	maxInventorySize = 10;
}

/***********************************************************/
/*!
\brief
	Adds the Item to the Inventory

\param ShelfObject
	Adds the Object on the Shelf to the inventory
*/
/***********************************************************/
void Ccharacter::AddToInventory(CModel::GEOMETRY_TYPE ShelfObject)
{
	if(inventorySize != maxInventorySize)
	{
		Inventory.push_back(ShelfObject);
		inventorySize++;
	}
}

void Ccharacter::RemoveFromInventory(int index)
{
	Inventory.erase(Inventory.begin()+index);
	inventorySize--;
}

/***********************************************************/
/*!
\brief
	Gets the Inventory

\return
	Returns the inventory
*/
/***********************************************************/
vector<CModel::GEOMETRY_TYPE> Ccharacter::GetInterventory()
{
	return Inventory;
}

/***********************************************************/
/*!
\brief
	Sets the Characters position, camera and target 

\param pos
	Sets the Characters Position

\param Cam
	Sets the Characters Camera

\param Tar
	Sets the Characters Target
*/
/***********************************************************/
void Ccharacter::SetCharacterPosCamTar(Vector3 Pos, Vector3 Cam, Vector3 Tar)
{
	Position = Pos;
	firstpersoncamera.position = Cam;
	firstpersoncamera.target = Tar;
	firstpersoncamera.UpdateUp(Cam,Tar);
}

Vector3 Ccharacter::getBoundMax()
{
	return BoundCheck.getBoundMax();
}

Vector3 Ccharacter::getBoundMin()
{
	return BoundCheck.getBoundMin();
}

void Ccharacter::ResetInventory()
{
	Inventory.clear();
	inventorySize = 0;
}