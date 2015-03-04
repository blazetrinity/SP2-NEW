/******************************************************************************/
/*!
\file	Material.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Struct to define a Material
*/
/******************************************************************************/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vertex.h"

/******************************************************************************/
/*!
		Struct Component:
\brief	Defines a Component and its methods
*/
/******************************************************************************/

struct Component
{
	float r,g,b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}

	void Set(float r, float g, float b)
	{
		this -> r=r;
		this -> g=g;
		this -> b=b;
	}
};

/******************************************************************************/
/*!
		Struct Material:
\brief	Defines a Material and its methods
*/
/******************************************************************************/
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;

	Material()
	{
		kAmbient.Set(0.1f, 0.1f, 0.1f);
		kDiffuse.Set(0.6f, 0.6f, 0.6f);
		kSpecular.Set(0.2f, 0.2f, 0.2f);
		kShininess = 9.f;
	}
};

#endif