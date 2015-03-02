/******************************************************************************/
/*!
\file	SceneObj.cpp
\author Malcolm
\par	email: Malcolm\@nyp.edu.sg
\brief
Define the Scene Objects and it's method
*/
/******************************************************************************/

#include "SceneObj.h"

/***********************************************************/
/*!
\brief
	CSceneObj default constructor
*/
/***********************************************************/
CSceneObj::CSceneObj(void)
{
	Rotate.SetToIdentity();
	Translate.Set(0, 0, 0);
	Scale.Set(1, 1, 1);
}

/***********************************************************/
/*!
\brief
	CSceneObj deconstructor
*/
/***********************************************************/
CSceneObj::~CSceneObj(void)
{
}

/***********************************************************/
/*!
\brief
	Sets the model, translate and bound to the Obj

\param Model
	Sets the Type of Model

\param Translate
	Sets the translate to the model

\param Scale
	Sets the scale to the model

\param Rotate
	Sets the Rotate to the model

\param Min
	Sets the minimum bound to the model

\param Max
	Sets the maximum bound to the model

\param level
	Sets the level for the model to be rendered in

\param type
	Sets the Obejct type
*/
/***********************************************************/
void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type)
{
	model.SetModel(Model);
	Translate = translate;
	Scale = scale;
	Rotate = rotate;
	Level = level;
	boundCheck.setBound(Min, Max);
	Type = type;
}

/***********************************************************/
/*!
\brief
	Sets the model, translate and bound to the Obj

\param Model
	Sets the Type of Model

\param Translate
	Sets the translate to the model

\param Scale
	Sets the scale to the model

\param Rotate
	Sets the Rotate to the model

\param Min
	Sets the minimum bound to the model

\param Max
	Sets the maximum bound to the model

\param level
	Sets the level for the model to be rendered in

\param type
	Sets the Obejct type

\param item
	Sets the Item Type
*/
/***********************************************************/
void CSceneObj::Set(CModel::GEOMETRY_TYPE Model,Vector3 translate, Vector3 scale, Mtx44 rotate, Vector3 Min, Vector3 Max, int level, CSceneObj::OBJ_TYPE type, CModel::GEOMETRY_TYPE Item)
{
	model.SetModel(Model);
	Translate = translate;
	Scale = scale;
	Rotate = rotate;
	Level = level;
	boundCheck.setBound(Min, Max);
	Type = type;
	item.SetModel(Item);
}

/***********************************************************/
/*!
\brief
	Sets the Model

\param Model
	Sets the type of model
*/
/***********************************************************/
void CSceneObj::setModel(CModel::GEOMETRY_TYPE Model){
	model.SetModel(Model);
}

/***********************************************************/
/*!
\brief
	Sets the Rotate

\param angle
	Sets the rotation value

\param x
	Sets the Rotation for X value

\param y
	Sets the Rotation for Y Value

\param z
	Sets the Rotation for Z Value
*/
/***********************************************************/
void CSceneObj::setRotate(int angle, bool x, bool y, bool z){
	Rotate.SetToRotation(angle, x, y, z);
}

/***********************************************************/
/*!
\brief
	Sets the Translate

\param angle
	Sets the Translation value

\param x
	Sets the Translation for X value

\param y
	Sets the Translation for Y Value

\param z
	Sets the Translation for Z Value
*/
/***********************************************************/
void CSceneObj::setTranslate(int x, int y, int z){
	Translate.x = x;
	Translate.y = y;
	Translate.z = z;
}

/***********************************************************/
/*!
\brief
	Sets the Scale

\param angle
	Sets the Scale value

\param x
	Sets the Scale for X value

\param y
	Sets the Scale for Y Value

\param z
	Sets the Scale for Z Value
*/
/***********************************************************/
void CSceneObj::setScale(int x, int y, int z){
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

/***********************************************************/
/*!
\brief
	Sets the bound for the Model

\param Min
	Sets the minimum Bound

\param Max
	Sets the Maximum bound
*/
/***********************************************************/
void CSceneObj::setBound(Vector3 Min, Vector3 Max){
	boundCheck.setBound(Min, Max);
}

/***********************************************************/
/*!
\brief
	Sets the Level for the Model to be rendered in

\param level
	Sets the Level for the Model to be rendered in
*/
/***********************************************************/
void CSceneObj::setLevel(int level)
{
	Level = level;
}

/***********************************************************/
/*!
\brief
	Sets the Object Type

\param type
	Sets the Type of Object
*/
/***********************************************************/
void CSceneObj::setOBJType(OBJ_TYPE type)
{
	Type = type;
}

/***********************************************************/
/*!
\brief
	Gets the Model

\return
	Returns the Model 
*/
/***********************************************************/
CModel::GEOMETRY_TYPE CSceneObj::getModel(void){
	return model.getModel();
}

/***********************************************************/
/*!
\brief
	Gets the Item

\return
	Returns the Item
*/
/***********************************************************/
CModel::GEOMETRY_TYPE CSceneObj::getItem()
{
	return item.getModel();
}

/***********************************************************/
/*!
\brief
	Gets the Rotation for the Model

\return
	Returns the Rotation for the model
*/
/***********************************************************/
Mtx44 CSceneObj::getRotate(void){
	return Rotate;
}

/***********************************************************/
/*!
\brief
	Gets the Translation for the Model

\return
	Returns the Translation for the model
*/
/***********************************************************/
Vector3 CSceneObj::getTranslate(void){
	return Translate;
}

/***********************************************************/
/*!
\brief
	Gets the Scale for the Model

\return
	Returns the Scale for the model
*/
/***********************************************************/
Vector3 CSceneObj::getScale(void){
	return Scale;
}

/***********************************************************/
/*!
\brief
	Gets the Minimum Bound for the Model

\return
	Returns the Minimum Bound for the model
*/
/***********************************************************/
Vector3 CSceneObj::getBoundMin(void){
	return boundCheck.getBoundMin();
}

/***********************************************************/
/*!
\brief
	Gets the Maximum Bound for the Model

\return
	Returns the Maximum Bound for the model
*/
/***********************************************************/
Vector3 CSceneObj::getBoundMax(void){
	return boundCheck.getBoundMax();
}

/***********************************************************/
/*!
\brief
	Gets the Level

\return
	Returns the Level
*/
/***********************************************************/
int CSceneObj::getLevel()
{
	return Level;
}

/***********************************************************/
/*!
\brief
	Gets the OBJ Type

\return
	Returns the Obj Type
*/
/***********************************************************/
CSceneObj::OBJ_TYPE CSceneObj::getOBJType()
{
	return Type;
}
