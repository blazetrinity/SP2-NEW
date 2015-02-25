#include "Character.h"

Ccharacter::Ccharacter()
{
	Position.Set(-120,-50,-100);
	Displacement.Set(0,0,0);
	Rotation.SetToIdentity();
	turnbody = 0;
	movebody = 0;
	firstpersoncamera.Init(Vector3(Position.x,Position.y + 30,Position.z), Vector3(Position.x, Position.y + 30, Position.z-10), Vector3(0, 1, 0));
	level = 1;
	inventorySize = 0;
	maxInventorySize = 2;
}
	
Ccharacter::~Ccharacter()
{

}
	
void Ccharacter::Update(double dt,vector<CSceneObj> Objs)
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

		updatePosition = BoundChecking(Objs);

		movebody = 0;
	}
	
	if(Application::IsKeyPressed('S'))
	{
		movebody += (float)(Move * dt);
		Displacement.Set(0,0,movebody);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		updatePosition = BoundChecking(Objs);

		movebody = 0;
	}

	if(Application::IsKeyPressed('D'))
	{
		movebody += (float)(Move * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		updatePosition = BoundChecking(Objs);

		movebody = 0;
	}

	if(Application::IsKeyPressed('A'))
	{
		movebody -= (float)(Move * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement = Rotation * Displacement;
		tempPosition += Displacement;

		updatePosition = BoundChecking(Objs);

		movebody = 0;
	}

	if(Application::IsKeyPressed(VK_LEFT))
	{
		turnbody += (float)(Turn * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement =  Rotation * Displacement;
		tempPosition += Displacement;

		updatePosition = BoundChecking(Objs);
	}

	if(Application::IsKeyPressed(VK_RIGHT))
	{
		turnbody -= (float)(Turn * dt);
		Displacement.Set(movebody,0,0);
		Rotation.SetToRotation(turnbody,0,1,0);
		Displacement =  Rotation * Displacement;
		tempPosition += Displacement;

		updatePosition = BoundChecking(Objs);
	}

	firstpersoncamera.Update(dt);

	if(updatePosition)
	{
		Position += Displacement;
		firstpersoncamera.position = Position;
		firstpersoncamera.position.y += 30;
		firstpersoncamera.target += Displacement;

		std::cout << "Character" << Position.x << " " << Position.y << " " << Position.z << std::endl;
		std::cout << "Target" << firstpersoncamera.target.x << " " << firstpersoncamera.target.y << " " << firstpersoncamera.target.z << std::endl;
	}

	if(Application::IsKeyPressed('X'))
	{
		Position.Set(0,0,0);
	}
}

Vector3 Ccharacter::GetPosition()
{
	return Position;
}

Camera3 Ccharacter::GetCamera()
{
	return firstpersoncamera;
}

float Ccharacter::GetRotation()
{
	return turnbody;
}

bool Ccharacter::BoundChecking(vector<CSceneObj> Objs)
{	
	if(Objs.size() > 0)
	{
		for(int i = 0; i < Objs.size(); ++i)
		{
			if(tempPosition.x < Objs[i].getBoundMax().x && tempPosition.x > Objs[i].getBoundMin().x && tempPosition.z < Objs[i].getBoundMax().z && tempPosition.z > Objs[i].getBoundMin().z && Objs[i].getLevel() == level)
			{
				return false;
			}

			else if(i == (Objs.size()-1))
			{
				return true;
			}
		}
	}

	else
	{
		return true;
	}
}

void Ccharacter::setModel(CModel::GEOMETRY_TYPE setModel, CModel::GEOMETRY_TYPE setModelArm)
{
	model.SetModel(setModel);
	modelArm.SetModel(setModelArm);
}

CModel::GEOMETRY_TYPE Ccharacter::GetModel()
{
	return model.getModel();
}

CModel::GEOMETRY_TYPE Ccharacter::GetModelArm()
{
	return modelArm.getModel();
}

void Ccharacter::setModelPosition(float defaultX, float defaultY, float defaultZ)
{
	Position.Set(defaultX, defaultY, defaultZ);
}

int Ccharacter::getLevel()
{
	return level;
}

void Ccharacter::setLevel(int lvl)
{
	level = lvl;
}

void Ccharacter::AddToInventory(CModel::GEOMETRY_TYPE ShelfObject)
{
	if(inventorySize != maxInventorySize)
	{
		Inventory.push_back(ShelfObject);
	}
}

vector<CModel::GEOMETRY_TYPE> Ccharacter::GetInterventory()
{
	return Inventory;
}