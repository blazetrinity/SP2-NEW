/******************************************************************************/
/*!
\file	Scene.h
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define a Scene
*/
/******************************************************************************/

#ifndef SCENE_H
#define SCENE_H

/******************************************************************************/
/*!
		Class Scene:
\brief	Defines a Scene and its methods
*/

/******************************************************************************/
class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif