/******************************************************************************/
/*!
\file	Camera2.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Camera
*/
/******************************************************************************/

#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

/***********************************************************/
/*!
\brief
	Camera2 constructor
*/
/***********************************************************/
Camera2::Camera2()
{
}

/***********************************************************/
/*!
\brief
	Camera2 deconstructor
*/
/***********************************************************/
Camera2::~Camera2()
{
}

/***********************************************************/
/*!
\brief
	Initialises a camera

\param pos
	Sets the position for the camera

\param target
	Sets the target for the camera

\param up
	Sets the up view for the camera
*/
/***********************************************************/
void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

/***********************************************************/
/*!
\brief
	Updates the Camera

\param dt
	Using time to update the camera
*/
/***********************************************************/
void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if(Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if(direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if(Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

/***********************************************************/
/*!
\brief
	Resets the camera to its default position
*/
/***********************************************************/
void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}