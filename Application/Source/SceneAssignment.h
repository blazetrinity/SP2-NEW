#ifndef SCENEASSIGNMENT_H
#define SCENEASSIGNMENT_H

#include "Scene.h"
#include "Camera.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Material.h"
#include "Utility.h"
#include "Light.h"
#include "Character.h"
#include "SceneObj.h"
#include "Interaction.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class SceneAssignment : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS, //in case you missed out practical 7
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum SELECT
	{
		MENU,
		CASHIER,
		CUSTOMER,
		SECURITY,
		EXIT,
	};

	SceneAssignment();
	~SceneAssignment();

	static SELECT currentScene;
	
	string menu[6];

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	Mesh*meshList[CModel::NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Light lights[1];

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh *mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh *mesh, std::string text, Color color, float size, float x, float y);
	void RenderObjs();
	void RenderCharacter();
	void RenderMenu();
	void InitSkyboxLevel1();
	void InitLiftLevel1();
	void InitSkyboxLevel2();
	void InitLiftLevel2();
	void InitOBJs();
	void InitCharacterModel();
	void InitAIModel();
	void LiftInteraction(double dt);
	void InteractionCheck();

	float fps;
	float FloorTimer;
	float MoveDoor;
	
	bool SecurityCamera;

	int CameraMode;

	int highlight;
	int delay;

	vector<int> Doorindex;
	int MoveDoorUpperLimit;
	int MoveDoorLowerLimit;

	CInteraction LiftDoor, Lift;

	Ccharacter Character;
	
	Camera3 SecurityCamera1, SecurityCamera2, SecurityCamera3, SecurityCamera4;

	Camera3 PanCam;

	CSceneObj myObj;
	vector<CSceneObj> Objs;

	Vector3 MaximumBound, MinimumBound;
	Vector3 Translate, Scale;
	Mtx44 Rotate;

	MS modelStack, viewStack, projectionStack;
};

#endif