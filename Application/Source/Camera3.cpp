/******************************************************************************/
/*!
\file	Camera3.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Camera
*/
/******************************************************************************/

#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

/***********************************************************/
/*!
\brief
	Camera2's constructor
*/
/***********************************************************/
Camera3::Camera3()
{
}

/***********************************************************/
/*!
\brief
	Camera2's destructor
*/
/***********************************************************/
Camera3::~Camera3()
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
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	defaultview = view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	rotateanglevertical = 0;
	rotateanglehorizontal = 0;
	rotatedirection = true;
}

/***********************************************************/
/*!
\brief
	Update the camera position based on key pressed

\param dt - the delta time since the last time the function was called
*/
/***********************************************************/
void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 25.f;
	// tilt up
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		
		if(rotateanglevertical < 0.5)
		{
			rotateanglevertical += dt;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = (rotation *(target - position) + position);
		}
	}

	// tilt down
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if(rotateanglevertical > -0.3)
		{
			rotateanglevertical -= dt;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = (rotation *(target - position) + position);
		}
	}

	// tilt left
	if(Application::IsKeyPressed(VK_LEFT))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (rotation *(target - position) + position);
		up = rotation * up;
	}

	// tilt right
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (rotation *(target - position) + position);
		up = rotation * up;
	}
}

/***********************************************************/
/*!
\brief
	Update the camera's up position based on key pressed

\param pos
	Camera's position

\param Tar
	Camera's Target
*/
/***********************************************************/
void Camera3::UpdateUp(Vector3 Pos, Vector3 Tar)
{
	view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

/***********************************************************/
/*!
\brief
	Updates the Camera Rotation

\param dt
	Using delta time to update the Camera's Rotation
*/
/***********************************************************/
void Camera3::CameraRotateUpdate(double dt)
{
	static const float CAMERA_SPEED = 10.f;

	// tilt left
	if(rotatedirection)
	{
		rotateanglehorizontal += (float)(CAMERA_SPEED * dt);
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (position + view);
		up = rotation * up;
	}

	// tilt right
	else
	{
		rotateanglehorizontal -= (float)(CAMERA_SPEED * dt);
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (position + view);
		up = rotation * up;
	}

	if(rotateanglehorizontal > 10 || rotateanglehorizontal < -10)
	{
		rotatedirection = !rotatedirection;
	}
}

/***********************************************************/
/*!
\brief
	Update the Camera Pan View

\param dt
	Using Delta Time to udate the camera's pan
*/
/***********************************************************/
void Camera3::UpdatePan(double dt)
{
	static const float CAMERA_SPEED = 25.f;
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (position + view);
		up = rotation * up;
}

/***********************************************************/
/*!
\brief
	Reset's the camera's variables to default
*/
/***********************************************************/
void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	view = defaultview;
}