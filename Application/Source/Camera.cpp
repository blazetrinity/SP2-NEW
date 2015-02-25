#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

/***********************************************************/
/*!
\brief
	Camera constructor
*/
/***********************************************************/
Camera::Camera()
{
	Reset();
}

/***********************************************************/
/*!
\brief
	Camera destructor
*/
/***********************************************************/
Camera::~Camera()
{
}

/***********************************************************/
/*
\brief
	Initialize the Camera variables values
\param pos - position of the camera

\param target - position of the target

\param up - the up direction of the camera's position
*/
/***********************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/***********************************************************/
/*!
\brief
	Resets the camera's properties's value
*/
/***********************************************************/
void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

/***********************************************************/
/*!
\brief
	Update the camera position based on key pressed
*/
/***********************************************************/
void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}