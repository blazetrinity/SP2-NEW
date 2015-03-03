/******************************************************************************/
/*!
\file	Model.h
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define a Model
*/
/******************************************************************************/

#ifndef MODEL_H
#define MODEL_H

/******************************************************************************/
/*!
		Class CModel:
\brief	Defines a Model and its methods
*/
/******************************************************************************/

class CModel
{
public:
	CModel(void);
	~CModel(void);

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LIGHTBALL,
		GEO_TEXT,
		GEO_Level1,
		GEO_ENTERANCE,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_ATM_WALL,
		GEO_WANTED_WALL,
		GEO_Level2,
		GEO_LIFT_WALL,
		GEO_LIFT_DOOR,
		GEO_LIFT_FLOOR1,
		GEO_LIFT_FLOOR2,
		GEO_STORAGE,
		GEO_COUNTER,
		GEO_RIGHTSHELFWCANS,
		GEO_RIGHTSHELFWMD,
		GEO_RIGHTSHELFWSODA,
		GEO_RIGHTSHELFWCEREAL,
		GEO_RIGHTSHELFWCEREAL1,
		GEO_RIGHTSHELFWCEREAL2,
		GEO_RIGHTSHELFWCEREAL3,
		GEO_RIGHTSHELFWBEANCAN,
		GEO_CASHIERC,
		GEO_FOODSTORAGE,
		GEO_FOODSTORAGE1,
		GEO_FOODSTORAGE2,
		GEO_FOODSTORAGE3,
		GEO_FOODSTORAGE4,
		GEO_ATM,
		GEO_TROLLEY,
		GEO_TABLE,
		GEO_MONITOR,
		GEO_CUSTOMER,
		GEO_CUSTOMERARM,
		GEO_CASHIER,
		GEO_CASHIERARM,
		GEO_SECURITY,
		GEO_SECURITYARM,
		GEO_FRUITSTALL,
		GEO_GANTRY,
		GEO_ADULT2,
		GEO_ARM2,
		GEO_ADULT3,
		GEO_ARM3,
		GEO_KIDB,
		GEO_KIDG,
		GEO_BOYARM,
		GEO_GIRLARM,
		GEO_CAMERA,
		GEO_CLEANER,
		GEO_GIRL_CLEANER,
		GEO_CLEANERARMS,
		GEO_GIRL_CLEANERARMS,
		GEO_SODA,
		GEO_GUMMYCEREAL,
		GEO_CEREALKBOX,
		GEO_STITCHCEREAL,
		GEO_BAKEDCANS,
		GEO_CHIPCEREAL,
		GEO_APPLE,
		GEO_ORANGE,
		GEO_HERSHEYS,
		GEO_FERRERO,
		GEO_COKEZERO,
		GEO_MOUNTAINDEW,
		GEO_SHRIMPBOX,
		GEO_ATMSCREEN,
		GEO_INVENT_RED,
		GEO_INVENT_YELLOW,
		GEO_BACKGROUND,
		NUM_GEOMETRY,
	};

	GEOMETRY_TYPE getModel();

	void SetModel(GEOMETRY_TYPE Model);

private:
	GEOMETRY_TYPE model;
};

#endif
