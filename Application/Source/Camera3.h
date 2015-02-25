/***********************************************************/
/*!
\file	Camera2.h
\author	Malcolm Lim
\par
\brief	Create a camera using Camera.h as it's parents
*/
/***********************************************************/
#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "Mtx44.h"

/***********************************************************/
/*!
	class Camera2
\brief	Defines a camera and it's method with class Camera's method inside
*/
/***********************************************************/

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 defaultview;

	Mtx44 rotation;

	float rotateanglevertical;
	float rotateanglehorizontal;

	bool rotatedirection;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	void CameraRotateUpdate(double dt);
	void UpdatePan(double dt);
};

#endif