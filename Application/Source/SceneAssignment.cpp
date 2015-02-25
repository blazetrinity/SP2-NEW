#include "SceneAssignment.h"
#include "GL\glew.h"
#include <sstream>

#include "shader.hpp"
#include "Mtx44.h"
#include "LoadTGA.h"
#include "Application.h"
#include "MeshBuilder.h"

SceneAssignment::SELECT SceneAssignment::currentScene = MENU;

SceneAssignment::SceneAssignment()
{
}

SceneAssignment::~SceneAssignment()
{
}

void SceneAssignment::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//MultiLight.fragmentshader" );
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 450, 1000);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 70;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//Initialize camera settings
	//Paning camera 
	PanCam.Init(Vector3(0,45,0),Vector3(15, 0, 0),Vector3(0,1,0));

	//Initialize camera settings
	SecurityCamera1.Init(Vector3(-140,45,-170),Vector3(0,0,0),Vector3(0,1,0));
	SecurityCamera2.Init(Vector3(140,45,90),Vector3(0,0,0),Vector3(0,1,0));
	SecurityCamera3.Init(Vector3(-140,45,90),Vector3(0,0,0),Vector3(0,1,0));
	SecurityCamera4.Init(Vector3(140,45,-90),Vector3(0,0,0),Vector3(0,1,0));

	meshList[CModel::GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);

	meshList[CModel::GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", 1 ,Color(1,1,1));

	meshList[CModel::GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[CModel::GEO_TEXT] ->textureID = LoadTGA("Image//Fixedsys.tga");
	
	InitSkyboxLevel1();
	InitSkyboxLevel2();
	InitOBJs();
	InitCharacterModel();

	LiftDoor.setLevel(1);
	LiftDoor.setInteractionBound(Vector3(0,-1,-190),Vector3(75,1,-90));

	Lift.setLevel(1);
	Lift.setInteractionBound(Vector3(90,-1,-190),Vector3(140,1,-90));

	Mtx44 projection;
	projection.SetToPerspective(45.f,4.f/3.f,0.1f,10000.f);
	projectionStack.LoadMatrix(projection);
	
	//Initialize current scene
	currentScene = MENU; //show menu

	menu[0] = "MENU";
	menu[1] = "Select:";
	menu[2] = "-> Play as CASHIER";
	menu[3] = "-> Play as CUSTOMER";
	menu[4] = "-> Play as SECURITY";
	menu[5] = "-> Exit";

	CameraMode = -1;
	SecurityCamera = false;
	fps = 0;
	MoveDoor = 0;
	MoveDoorUpperLimit = 45;
	MoveDoorLowerLimit = 0;
	FloorTimer = 0;
	highlight = 2;
	delay = 0;
}

void SceneAssignment::InitSkyboxLevel1()
{
	meshList[CModel::GEO_ATM_WALL] = MeshBuilder::GenerateQuad("AtmWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_ATM_WALL]->textureID = LoadTGA("Image//AtmSymbol.tga");

	MaximumBound.Set(-40,50,-190);
	MinimumBound.Set(-160,-50,-210);
	Translate.Set(-100,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_ATM_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);
	
	Objs.push_back(myObj);
	

	//mall Level 1
	meshList[CModel::GEO_Level1] = MeshBuilder::GenerateQuad("mall Back", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_Level1]->textureID = LoadTGA("Image//Skybox_Level1.tga");

	MaximumBound.Set(160,50,110);
	MinimumBound.Set(40,-50,90);
	Translate.Set(100,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(60,50,110);
	MinimumBound.Set(-60,-50,90);
	Translate.Set(0,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(-40,50,110);
	MinimumBound.Set(-160,-50,90);
	Translate.Set(-100,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(160,50,110);
	MinimumBound.Set(140,-50,-10);
	Translate.Set(150,0,50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(160,50,-10);
	MinimumBound.Set(140,-50,-110);
	Translate.Set(150,0,-50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(60,50,-190);
	MinimumBound.Set(-60,-50,-210);
	Translate.Set(0,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_Level1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////mall top
	meshList[CModel::GEO_TOP] = MeshBuilder::GenerateQuad("mall top", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_TOP]->textureID = LoadTGA("Image//Skybox_Top.tga");

	//MaximumBound.Set(450,60,50);
	//MinimumBound.Set(150,40,-50);
	Translate.Set(0,50,-50);
	Scale.Set(300,300,300);
	Rotate.SetToRotation(90,1,0,0);
	
	myObj.Set(CModel::GEO_TOP, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////mall bottom
	meshList[CModel::GEO_BOTTOM] = MeshBuilder::GenerateQuad("mall Bottom", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox_Bottom.tga");

	//MaximumBound.Set(150,-40,150);
	//MinimumBound.Set(-150,-60,-150);
	Translate.Set(0,-50,-50);
	Scale.Set(300,300,300);
	Rotate.SetToRotation(-90,1,0,0);

	myObj.Set(CModel::GEO_BOTTOM, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////mall ENTERANCE
	meshList[CModel::GEO_ENTERANCE] = MeshBuilder::GenerateQuad("mall rght", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_ENTERANCE]->textureID = LoadTGA("Image//Skybox_Front.tga");

	MaximumBound.Set(-140,50,110);
	MinimumBound.Set(-160,-50,-10);
	Translate.Set(-150,0,50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_ENTERANCE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(-140,50,10);
	MinimumBound.Set(-160,-50,-110);
	Translate.Set(-150,0,-50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_ENTERANCE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(-140,50,-90);
	MinimumBound.Set(-160,-50,-210);
	Translate.Set(-150,0,-150);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_ENTERANCE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	InitLiftLevel1();
}

void SceneAssignment::InitLiftLevel1()
{
	////Lift Wall Left from inside the lift
	meshList[CModel::GEO_LIFT_WALL] = MeshBuilder::GenerateQuad("LiftWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_LIFT_WALL]->textureID = LoadTGA("Image//Skybox_Front.tga");

	MaximumBound.Set(150,50,-90);
	MinimumBound.Set(50,-50,-110);
	Translate.Set(100,0,-100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);
	
	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);
	
	Objs.push_back(myObj);

	////Lift Wall Left from inside the lift
	MaximumBound.Set(150,50,-90);
	MinimumBound.Set(50,-50,-110);
	Translate.Set(100,0,-100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);
	
	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	MaximumBound.Set(160,50,-110);
	MinimumBound.Set(140,-50,-210);
	Translate.Set(150,0,-150);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////Lift Wall Right from inside the lift
	MaximumBound.Set(160,50,-190);
	MinimumBound.Set(40,-50,-210);
	Translate.Set(100,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Back from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-150);
	Translate.Set(50,40,-175);
	Scale.Set(50,25,50);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-150);
	Translate.Set(50,40,-175);
	Scale.Set(50,25,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	meshList[CModel::GEO_LIFT_DOOR] = MeshBuilder::GenerateOBJ("/Lift", "OBJ//LiftDoor.obj");
	meshList[CModel::GEO_LIFT_DOOR]->textureID = LoadTGA("Image//LiftTexture.tga");

	//MaximumBound.Set(60,50,-100);
	//MinimumBound.Set(40,-50,-150);
	Translate.Set(50,-50,-175);
	Scale.Set(7,7,3);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_DOOR, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::DOOR);

	Objs.push_back(myObj);
}

void SceneAssignment::InitSkyboxLevel2()
{
	//mall Level2
	meshList[CModel::GEO_Level2] = MeshBuilder::GenerateQuad("mall Back", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_Level2]->textureID = LoadTGA("Image//Skybox_Level2.tga");

	MaximumBound.Set(160,50,110);
	MinimumBound.Set(40,-50,90);
	Translate.Set(100,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(60,50,110);
	MinimumBound.Set(-60,-50,90);
	Translate.Set(0,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(-40,50,110);
	MinimumBound.Set(-160,-50,90);
	Translate.Set(-100,0,100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(160,50,110);
	MinimumBound.Set(140,-50,-10);
	Translate.Set(150,0,50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);
	
	Objs.push_back(myObj);

	//
	MaximumBound.Set(160,50,-10);
	MinimumBound.Set(140,-50,-110);
	Translate.Set(150,0,-50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(-140,50,110);
	MinimumBound.Set(-160,-50,-10);
	Translate.Set(-150,0,50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(-140,50,10);
	MinimumBound.Set(-160,-50,-110);
	Translate.Set(-150,0,-50);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(-140,50,-90);
	MinimumBound.Set(-160,-50,-210);
	Translate.Set(-150,0,-150);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	//
	MaximumBound.Set(-40,50,-190);
	MinimumBound.Set(-160,-50,-210);
	Translate.Set(-100,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	//
	MaximumBound.Set(60,50,-190);
	MinimumBound.Set(-60,-50,-210);
	Translate.Set(0,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_Level2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////mall top
	meshList[CModel::GEO_TOP] = MeshBuilder::GenerateQuad("mall top", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_TOP]->textureID = LoadTGA("Image//Skybox_Top.tga");

	//MaximumBound.Set(450,60,50);
	//MinimumBound.Set(150,40,-50);
	Translate.Set(0,50,-50);
	Scale.Set(300,300,300);
	Rotate.SetToRotation(90,1,0,0);

	myObj.Set(CModel::GEO_TOP, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);
	
	Objs.push_back(myObj);

	////mall bottom
	meshList[CModel::GEO_BOTTOM] = MeshBuilder::GenerateQuad("mall Bottom", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox_Bottom.tga");

	//MaximumBound.Set(150,-40,150);
	//MinimumBound.Set(-150,-60,-150);
	Translate.Set(0,-50,-50);
	Scale.Set(300,300,300);
	Rotate.SetToRotation(-90,1,0,0);

	myObj.Set(CModel::GEO_BOTTOM, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	InitLiftLevel2();
}

void SceneAssignment::InitLiftLevel2()
{
	////Lift Wall Left from inside the lift
	meshList[CModel::GEO_LIFT_WALL] = MeshBuilder::GenerateQuad("LiftWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_LIFT_WALL]->textureID = LoadTGA("Image//Skybox_Front.tga");

	MaximumBound.Set(150,50,-90);
	MinimumBound.Set(50,-50,-110);
	Translate.Set(100,0,-100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);
	
	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);
	
	Objs.push_back(myObj);

	////Lift Wall Left from inside the lift
	MaximumBound.Set(150,50,-90);
	MinimumBound.Set(50,-50,-110);
	Translate.Set(100,0,-100);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(180,0,1,0);
	
	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	MaximumBound.Set(160,50,-110);
	MinimumBound.Set(140,-50,-210);
	Translate.Set(150,0,-150);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////Lift Wall Right from inside the lift
	MaximumBound.Set(160,50,-190);
	MinimumBound.Set(40,-50,-210);
	Translate.Set(100,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Back from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);
	
	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-150);
	Translate.Set(50,40,-175);
	Scale.Set(50,25,50);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-150);
	Translate.Set(50,40,-175);
	Scale.Set(50,25,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

	Objs.push_back(myObj);

	////Lift Wall Front from inside the lift
	meshList[CModel::GEO_LIFT_DOOR] = MeshBuilder::GenerateOBJ("/Lift", "OBJ//LiftDoor.obj");
	meshList[CModel::GEO_LIFT_DOOR]->textureID = LoadTGA("Image//LiftTexture.tga");

	//MaximumBound.Set(60,50,-100);
	//MinimumBound.Set(40,-50,-150);
	Translate.Set(50,-50,-175);
	Scale.Set(7,7,3);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_DOOR, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::DOOR);

	Objs.push_back(myObj);
}

void SceneAssignment::InitOBJs()
{
	////Cashier3
	meshList[CModel::GEO_COUNTER] = MeshBuilder::GenerateOBJ("cashier", "OBJ//Cashier.obj");
	meshList[CModel::GEO_COUNTER]->textureID = LoadTGA("Image//cashierCounter.tga");

	MaximumBound.Set(-52, 50, 0);
	MinimumBound.Set(-130, -50, -62);
	Translate.Set(-90,-50,-20);
	Scale.Set(6,6,6);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_COUNTER, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::COUNTER);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_RIGHTSHELFWCANS] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfWCans.obj");
	meshList[CModel::GEO_RIGHTSHELFWCANS]->textureID = LoadTGA("Image//CanTexture3.tga");

	MaximumBound.Set(60, 50, 1);
	MinimumBound.Set(-20, -50, -30);
	Translate.Set(20,-50,-30);
	Scale.Set(10,10,10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCANS, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_RIGHTSHELFWMD] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwMD.obj");
	meshList[CModel::GEO_RIGHTSHELFWMD]->textureID = LoadTGA("Image//CanTexture2.tga");

	MaximumBound.Set(60,50,-29);
	MinimumBound.Set(-20,-50,-60);
	Translate.Set(20,-50,-30);
	Scale.Set(10,10,10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWMD, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_RIGHTSHELFWSODA] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwSoda.obj");
	meshList[CModel::GEO_RIGHTSHELFWSODA]->textureID = LoadTGA("Image//CanTexture.tga");

	MaximumBound.Set(160, 50, 1);
	MinimumBound.Set(80,-50,-30);
	Translate.Set(120,-50,-30);
	Scale.Set(10,10,10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWSODA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf4
	meshList[CModel::GEO_RIGHTSHELFWCEREAL] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwCereal.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL]->textureID = LoadTGA("Image//cerealBox.tga");

	MaximumBound.Set(160, 50, -29);
	MinimumBound.Set(80, -50, -60);
	Translate.Set(120, -50, -30);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Cashier4
	meshList[CModel::GEO_COUNTER] = MeshBuilder::GenerateOBJ("cashier", "OBJ//Cashier.obj");
	meshList[CModel::GEO_COUNTER]->textureID = LoadTGA("Image//cashierCounter.tga");

	MaximumBound.Set(-52, 50, 80);
	MinimumBound.Set(-130, -50, 18);
	Translate.Set(-90, -50, 60);
	Scale.Set(6, 6, 6);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_COUNTER, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::COUNTER);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_RIGHTSHELFWCEREAL1] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwCereal1.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL1]->textureID = LoadTGA("Image//cerealBox1.tga");

	MaximumBound.Set(60, 50, 81);
	MinimumBound.Set(-20, -50, 50);
	Translate.Set(20, -50, 50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_RIGHTSHELFWCEREAL2] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwCereal2.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL2]->textureID = LoadTGA("Image//displayTable2.tga");

	MaximumBound.Set(60, 50, 51);
	MinimumBound.Set(-20, -50, 15);
	Translate.Set(20, -50, 50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_RIGHTSHELFWCEREAL3] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwCereal3.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL3]->textureID = LoadTGA("Image//stitchCereal.tga");

	MaximumBound.Set(160, 50,81);
	MinimumBound.Set(80, -50, 50);
	Translate.Set(120, -50, 50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,1,0,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL3, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf4
	meshList[CModel::GEO_RIGHTSHELFWBEANCAN] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwBeanCan.obj");
	meshList[CModel::GEO_RIGHTSHELFWBEANCAN]->textureID = LoadTGA("Image//beanCan.tga");

	MaximumBound.Set(160, 50, 51);
	MinimumBound.Set(80, -50, 15);
	Translate.Set(120, -50, 50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWBEANCAN, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Trolley1
	meshList[CModel::GEO_TROLLEY] = MeshBuilder::GenerateOBJ("trolley", "OBJ//trolley.obj");
	meshList[CModel::GEO_TROLLEY]->textureID = LoadTGA("Image//trolley.tga");

	MaximumBound.Set(-63, 50, -143);
	MinimumBound.Set(-139, -50, -189);

	myObj.setModel(CModel::GEO_TROLLEY);
	myObj.setTranslate(-125, -35, -180);
	myObj.setScale(15, 15, 15);
	myObj.setRotate(180,0,1,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(1);
	myObj.setOBJType(CSceneObj::CART);

	Objs.push_back(myObj);

	////Trolley2
	meshList[CModel::GEO_TROLLEY2] = MeshBuilder::GenerateOBJ("trolley", "OBJ//trolley.obj");
	meshList[CModel::GEO_TROLLEY2]->textureID = LoadTGA("Image//trolley.tga");

	/*MaximumBound.Set(54, 50, -3);
	MinimumBound.Set(-12, -50, -28);*/

	myObj.setModel(CModel::GEO_TROLLEY2);
	myObj.setTranslate(-115, -35, -180);
	myObj.setScale(15, 15, 15);
	myObj.setRotate(180,0,1,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(1);
	myObj.setOBJType(CSceneObj::CART);

	Objs.push_back(myObj);

	////Trolley3
	meshList[CModel::GEO_TROLLEY2] = MeshBuilder::GenerateOBJ("trolley", "OBJ//trolley.obj");
	meshList[CModel::GEO_TROLLEY2]->textureID = LoadTGA("Image//trolley.tga");

	/*MaximumBound.Set(54, 50, -3);
	MinimumBound.Set(-12, -50, -28);*/

	myObj.setModel(CModel::GEO_TROLLEY2);
	myObj.setTranslate(-105, -35, -180);
	myObj.setScale(15, 15, 15);
	myObj.setRotate(180,0,1,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(1);
	myObj.setOBJType(CSceneObj::CART);

	Objs.push_back(myObj);

	////ATM Machine
	meshList[CModel::GEO_ATM] = MeshBuilder::GenerateOBJ("ATM", "OBJ//ATM.obj");
	meshList[CModel::GEO_ATM]->textureID = LoadTGA("Image//ATMTexture.tga");

	MaximumBound.Set(-65, 50, -181);
	MinimumBound.Set(-94, -50, -190);
	Translate.Set(-80, -50, -190);
	Scale.Set(8, 10, 8);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_ATM, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::ATM);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_FOODSTORAGE] = MeshBuilder::GenerateOBJ("foodstorage", "OBJ//FoodStorage.obj");
	meshList[CModel::GEO_FOODSTORAGE]->textureID = LoadTGA("Image//FoodStorage.tga");

	MaximumBound.Set(142, 50, 109);
	MinimumBound.Set(72, -50, 45);
	Translate.Set(115, -50, 80);
	Scale.Set(7, 8, 7);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_FOODSTORAGE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_FOODSTORAGE1] = MeshBuilder::GenerateOBJ("foodstorage1", "OBJ//FoodStorage1.obj");
	meshList[CModel::GEO_FOODSTORAGE1]->textureID = LoadTGA("Image//FoodStorage1.tga");

	MaximumBound.Set(-89, 50, 109);
	MinimumBound.Set(-159, -50, 45);
	Translate.Set(-116, -50, 80);
	Scale.Set(7, 8, 7);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_FOODSTORAGE1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_FOODSTORAGE2] = MeshBuilder::GenerateOBJ("foodstorage2", "OBJ//FoodStorage2.obj");
	meshList[CModel::GEO_FOODSTORAGE2]->textureID = LoadTGA("Image//FoodStorage2.tga");

	MaximumBound.Set(140, 50, -49);
	MinimumBound.Set(70, -50, -87);
	Translate.Set(113,-50,-78);
	Scale.Set(7, 8, 7);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_FOODSTORAGE2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_RIGHTSHELFWCANS] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfWCans.obj");
	meshList[CModel::GEO_RIGHTSHELFWCANS]->textureID = LoadTGA("Image//CanTexture3.tga");

	MaximumBound.Set(139, 50, 31);
	MinimumBound.Set(81, -50, 0);

	myObj.setModel(CModel::GEO_RIGHTSHELFWCANS);
	myObj.setTranslate(120, -50, -1);
	myObj.setScale(10, 10, 10);
	myObj.setRotate(0,1,0,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(2);
	myObj.setOBJType(CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_RIGHTSHELFWMD] = MeshBuilder::GenerateOBJ("RightShelf", "OBJ//RightShelfwMD.obj");
	meshList[CModel::GEO_RIGHTSHELFWMD]->textureID = LoadTGA("Image//CanTexture2.tga");

	MaximumBound.Set(139,50,0);
	MinimumBound.Set(81,-50,-29);

	myObj.setModel(CModel::GEO_RIGHTSHELFWMD);
	myObj.setTranslate(120, -50, 1);
	myObj.setScale(10, 10, 10);
	myObj.setRotate(180,0,1,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(2);
	myObj.setOBJType(CSceneObj::SHELF);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_RIGHTSHELFWCANS] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfWCans.obj");
	meshList[CModel::GEO_RIGHTSHELFWCANS]->textureID = LoadTGA("Image//CanTexture3.tga");

	MaximumBound.Set(-81, 50, 30);
	MinimumBound.Set(-139, -50, -1);

	myObj.setModel(CModel::GEO_RIGHTSHELFWCANS);
	myObj.setTranslate(-120, -50, -1);
	myObj.setScale(10, 10, 10);
	myObj.setRotate(0,1,0,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(2);
	myObj.setOBJType(CSceneObj::SHELF);

	Objs.push_back(myObj);


	////Shelf4
	meshList[CModel::GEO_RIGHTSHELFWMD] = MeshBuilder::GenerateOBJ("RightShelf", "OBJ//RightShelfwMD.obj");
	meshList[CModel::GEO_RIGHTSHELFWMD]->textureID = LoadTGA("Image//CanTexture2.tga");

	MaximumBound.Set(-82,50, -1);
	MinimumBound.Set(-139,-50,-29);

	myObj.setModel(CModel::GEO_RIGHTSHELFWMD);
	myObj.setTranslate(-120, -50, 1);
	myObj.setScale(10, 10, 10);
	myObj.setRotate(180,0,1,0);
	myObj.setBound(MinimumBound, MaximumBound);
	myObj.setLevel(2);
	myObj.setOBJType(CSceneObj::SHELF);

	Objs.push_back(myObj);
}

void SceneAssignment::InitCharacterModel()
{
	meshList[CModel::GEO_CUSTOMER] = MeshBuilder::GenerateOBJ("Adult shopper 1", "OBJ//adult_male_body.obj");
	meshList[CModel::GEO_CUSTOMER] ->textureID = LoadTGA("Image//shopper1_uv.tga");

	meshList[CModel::GEO_CUSTOMERARM] = MeshBuilder::GenerateOBJ("Adult shopper 1's arm", "OBJ//arm.obj");
	meshList[CModel::GEO_CUSTOMERARM] ->textureID = LoadTGA("Image//shopper1_arm.tga");

	meshList[CModel::GEO_CASHIER] = MeshBuilder::GenerateOBJ("cashier", "OBJ//adult_male_body.obj");
	meshList[CModel::GEO_CASHIER] ->textureID = LoadTGA("Image//cashier_uv.tga");

	meshList[CModel::GEO_CASHIERARM] = MeshBuilder::GenerateOBJ("Cashier's arm", "OBJ//arm.OBJ");
	meshList[CModel::GEO_CASHIERARM] ->textureID = LoadTGA("Image//cashier_arm.tga");
	
	meshList[CModel::GEO_SECURITY] = MeshBuilder::GenerateOBJ("security guard", "OBJ//adult_male_body.OBJ");
	meshList[CModel::GEO_SECURITY] ->textureID = LoadTGA("Image//security_uv.tga");

	meshList[CModel::GEO_SECURITYARM] = MeshBuilder::GenerateOBJ("security's arm", "OBJ//arm.OBJ");
	meshList[CModel::GEO_SECURITYARM] ->textureID = LoadTGA("Image//security_arm.tga");
}

void SceneAssignment::InitAIModel()
{
	/*
	meshList[CModel::GEO_ADULT2] = MeshBuilder::GenerateOBJ("Adult shopper 2", "OBJ//adult_male_body.obj");
	meshList[CModel::GEO_ADULT2] ->textureID = LoadTGA("Image//shopper2_uv.tga");

	meshList[CModel::GEO_ARM2] = MeshBuilder::GenerateOBJ("Adult shopper 2's arm", "OBJ//arm.obj");
	meshList[CModel::GEO_ARM2] ->textureID = LoadTGA("Image//shopper2_arm.tga");

	meshList[CModel::GEO_ADULT3] = MeshBuilder::GenerateOBJ("Adult shopper 3", "OBJ//adult_female_body.obj");
	meshList[CModel::GEO_ADULT3] ->textureID = LoadTGA("Image//shopper3_uv.tga");

	meshList[CModel::GEO_ARM3] = MeshBuilder::GenerateOBJ("Adult shopper 3's arm", "OBJ//arm.obj");
	meshList[CModel::GEO_ARM3] ->textureID = LoadTGA("Image//shopper3_arm.tga");
	
	meshList[CModel::GEO_KIDB] = MeshBuilder::GenerateOBJ("boy", "OBJ//kid_male_body.OBJ");
	meshList[CModel::GEO_KIDB] ->textureID = LoadTGA("Image//shopper2_uv.tga");

	meshList[CModel::GEO_BOYARM] = MeshBuilder::GenerateOBJ("Boy's arm", "OBJ//kid_arm.OBJ");
	meshList[CModel::GEO_BOYARM] ->textureID = LoadTGA("Image//shopper2_arm.tga");

	meshList[CModel::GEO_KIDG] = MeshBuilder::GenerateOBJ("girl", "OBJ//kid_female_body.OBJ");
	meshList[CModel::GEO_KIDG] ->textureID = LoadTGA("Image//kid_girl.tga");

	meshList[CModel::GEO_GIRLARM] = MeshBuilder::GenerateOBJ("Girl's arm", "OBJ//kid_arm.OBJ");
	meshList[CModel::GEO_GIRLARM] ->textureID = LoadTGA("Image//girl_arm.tga");
	*/
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

#define DOORSPEED 40.0f

void SceneAssignment::Update(double dt)
{
	if(Application::IsKeyPressed('7')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('8')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('9'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('0'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	
	if(Application::IsKeyPressed('E') && SecurityCamera == false)
	{
		SecurityCamera = true;
		CameraMode = 1;
	}
	
	else if(Application::IsKeyPressed('E') && SecurityCamera == true)
	{
		SecurityCamera = false;
		CameraMode = 0;
	}

	if(Application::IsKeyPressed('1') && SecurityCamera == true)
	{
		CameraMode = 1;
	}

	if(Application::IsKeyPressed('2') && SecurityCamera == true)
	{
		CameraMode = 2;
	}

	if(Application::IsKeyPressed('3') && SecurityCamera == true)
	{
		CameraMode = 3;
	}

	if(Application::IsKeyPressed('4') && SecurityCamera == true)
	{
		CameraMode = 4;
	}
	
	SecurityCamera1.CameraRotateUpdate(dt);
	SecurityCamera2.CameraRotateUpdate(dt);
	SecurityCamera3.CameraRotateUpdate(dt);
	SecurityCamera4.CameraRotateUpdate(dt);

	LiftInteraction(dt);

	//Menu
	if (currentScene == MENU)
	{
		if (Application::IsKeyPressed(VK_DOWN) && delay == 0 && highlight < 5)
		{
			highlight += 1;
			delay = 15;
		}
		if (Application::IsKeyPressed(VK_UP) && delay == 0 && highlight > 2)
		{
			highlight -= 1;
			delay = 15;
		}

		if (delay > 0)
		{
			--delay;
		}

		//selection of scene
		if (Application::IsKeyPressed(VK_RETURN))
		{
			int selection = highlight - 1;
			SELECT val = static_cast<SELECT>(selection);
			currentScene = val;
			CameraMode = 0;
		}

		if (currentScene == SECURITY)
		{
			Character.setModel(CModel::GEO_SECURITY,CModel::GEO_SECURITYARM);
			//Character.setModelPosition(7, 0, 9);
		}

		if (currentScene == CUSTOMER)
		{
			Character.setModel(CModel::GEO_CUSTOMER,CModel::GEO_CUSTOMERARM);
			//Character.setModelPosition(-3, 0, 9);
		}

		if (currentScene == CASHIER)
		{
			Character.setModel(CModel::GEO_CASHIER,CModel::GEO_CASHIERARM);
			//Character.setModelPosition(7, 0, 0);
		}
		PanCam.UpdatePan(dt);
	}

	if (currentScene != MENU)
	{

		if(Application::IsKeyPressed('1') && SecurityCamera == false)
		{
			Character.setModel(CModel::GEO_SECURITY,CModel::GEO_SECURITYARM);
			//Character.setModelPosition(7, 0, 9);
			currentScene = SECURITY;
		}

		if(Application::IsKeyPressed('2') && SecurityCamera == false)
		{
			Character.setModel(CModel::GEO_CUSTOMER,CModel::GEO_CUSTOMERARM);
			//Character.setModelPosition(-3, 0, 9);
			currentScene = CUSTOMER;
		}

		if(Application::IsKeyPressed('3') && SecurityCamera == false)
		{
			Character.setModel(CModel::GEO_CASHIER,CModel::GEO_CASHIERARM);
			//Character.setModelPosition(7, 0, 0);
			currentScene = CASHIER;
		}
		
		Character.Update(dt, Objs);
		InteractionCheck();
	}

	fps = 1/dt;
}

void SceneAssignment::InteractionCheck()
{
	if(Application::IsKeyPressed('K'))
	{
		for(int i = 0; i < Objs.size(); ++i)
		{
			if(Character.GetCamera().target.x > Objs[i].getBoundMin().x && Character.GetCamera().target.x < Objs[i].getBoundMax().x && Character.GetCamera().target.z > Objs[i].getBoundMin().z && Character.GetCamera().target.z < Objs[i].getBoundMax().z && Character.getLevel() == Objs[i].getLevel())
			{
				//Customer
				if(currentScene == CUSTOMER)
				{
					
				}

				//Cashier
				else if(currentScene == CASHIER)
				{
	
				}

				//Security
				else if(currentScene == SECURITY)
				{

				}
			}
		}	
	}
}

void SceneAssignment::LiftInteraction(double dt)
{
	if(Doorindex.size() == 0)
	{
		for(int i = 0; i < Objs.size(); ++i)
		{
			if(Objs[i].getModel() == CModel::GEO_LIFT_DOOR)
			{	
				Doorindex.push_back(i);	
			}
		}
	}

	if(Character.GetPosition().x > LiftDoor.getMinBound().x && Character.GetPosition().x < LiftDoor.getMaxBound().x && Character.GetPosition().z > LiftDoor.getMinBound().z && Character.GetPosition().z < LiftDoor.getMaxBound().z)
	{
		if(MoveDoor < MoveDoorUpperLimit)
		{
			MoveDoor += (float)(DOORSPEED * dt);
			Objs[Doorindex[0]].setTranslate(50,-50,-175+MoveDoor);
			Objs[Doorindex[1]].setTranslate(50,-50,-175+MoveDoor);
		}
	}

	else
	{
		if(MoveDoor > MoveDoorLowerLimit)
		{
			MoveDoor -= (float)(DOORSPEED * dt);
			Objs[Doorindex[0]].setTranslate(50,-50,-175+MoveDoor);
			Objs[Doorindex[1]].setTranslate(50,-50,-175+MoveDoor);
		}
	}

	FloorTimer += 10*dt;

	if(Application::IsKeyPressed('F') && Character.GetPosition().x > Lift.getMinBound().x && Character.GetPosition().x < Lift.getMaxBound().x && Character.GetPosition().z > Lift.getMinBound().z && Character.GetPosition().z < Lift.getMaxBound().z && FloorTimer > 20)
	{
		if(Character.getLevel() == 1)
		{
			Character.setLevel(2);
		}

		else if(Character.getLevel() == 2)
		{
			Character.setLevel(1);
		}
		FloorTimer = 0;
	}
}

void SceneAssignment::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 2);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SceneAssignment::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (CameraMode == -1)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		PanCam.position.x,PanCam.position.y,PanCam.position.z,
		PanCam.target.x,PanCam.target.y,PanCam.target.z,
		PanCam.up.x,PanCam.up.y,PanCam.up.z);

	}
	
	else if(CameraMode == 0)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		Character.GetCamera().position.x,Character.GetCamera().position.y,Character.GetCamera().position.z,
		Character.GetCamera().target.x,Character.GetCamera().target.y,Character.GetCamera().target.z,
		Character.GetCamera().up.x,Character.GetCamera().up.y,Character.GetCamera().up.z);
		
	}
	
	else if(CameraMode == 1)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		SecurityCamera1.position.x,SecurityCamera1.position.y,SecurityCamera1.position.z,
		SecurityCamera1.target.x,SecurityCamera1.target.y,SecurityCamera1.target.z,
		SecurityCamera1.up.x,SecurityCamera1.up.y,SecurityCamera1.up.z);
	}

	else if(CameraMode == 2)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		SecurityCamera2.position.x,SecurityCamera2.position.y,SecurityCamera2.position.z,
		SecurityCamera2.target.x,SecurityCamera2.target.y,SecurityCamera2.target.z,
		SecurityCamera2.up.x,SecurityCamera2.up.y,SecurityCamera2.up.z);
	}

	else if(CameraMode == 3)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		SecurityCamera3.position.x,SecurityCamera3.position.y,SecurityCamera3.position.z,
		SecurityCamera3.target.x,SecurityCamera3.target.y,SecurityCamera3.target.z,
		SecurityCamera3.up.x,SecurityCamera3.up.y,SecurityCamera3.up.z);
	}

	else if(CameraMode == 4)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(
		SecurityCamera4.position.x,SecurityCamera4.position.y,SecurityCamera4.position.z,
		SecurityCamera4.target.x,SecurityCamera4.target.y,SecurityCamera4.target.z,
		SecurityCamera4.up.x,SecurityCamera4.up.y,SecurityCamera4.up.z);
	}
	
	modelStack.LoadIdentity();

	//Light 1
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[CModel::GEO_AXES], false);

	RenderObjs();

	if (currentScene == MENU)
	{
		RenderMenu();
	}
	else
	{
		RenderCharacter();
	}

	std::ostringstream stringfps;

	stringfps << fps;

	RenderTextOnScreen(meshList[CModel::GEO_TEXT], "FPS:", Color(0, 1, 0), 3, 0, 1);

	RenderTextOnScreen(meshList[CModel::GEO_TEXT], stringfps.str(), Color(0, 1, 0), 3, 4, 1);
}

void SceneAssignment::RenderCharacter()
{
	modelStack.PushMatrix();
	modelStack.Translate(Character.GetPosition().x,Character.GetPosition().y,Character.GetPosition().z);
	modelStack.Rotate(Character.GetRotation(),0,1,0);
	modelStack.Scale(12,12,12);
	RenderMesh(meshList[Character.GetModel()], false);
	{
		modelStack.PushMatrix();
		{
			modelStack.PushMatrix();
			modelStack.Translate(-1.3, 0, 0);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[Character.GetModelArm()], false);
			modelStack.PopMatrix();
		}
		modelStack.Translate(1.3, 0, 0);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[Character.GetModelArm()], false);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
}

void SceneAssignment::RenderMenu()
{
	int a = 1;
	//menu
	for (int text = 0; text < 6; text++)
	{
		const float TextSize = 3;
		int x = 80/TextSize/2 - menu[text].length()/2;
		int y = 60/TextSize/2 + 3 - text;

		if(highlight == text)
		{
			a = 0;
		}

		RenderTextOnScreen(meshList[CModel::GEO_TEXT], menu[text], Color(1, a, a), TextSize, x, y);

		if(highlight == text)
		{
			a = 1;
		}

	}
}

void SceneAssignment::RenderObjs()
{
	for(int i = 0; i <Objs.size(); ++i)
	{
		if(Character.getLevel() == Objs[i].getLevel())
		{
			modelStack.PushMatrix();
			modelStack.Translate(Objs[i].getTranslate().x, Objs[i].getTranslate().y, Objs[i].getTranslate().z);
			modelStack.MultMatrix(Objs[i].getRotate());
			modelStack.Scale(Objs[i].getScale().x, Objs[i].getScale().y, Objs[i].getScale().z);
			RenderMesh(meshList[Objs[i].getModel()],false);
			modelStack.PopMatrix();
		}
	}
}

void SceneAssignment::RenderText(Mesh *mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneAssignment::RenderTextOnScreen(Mesh *mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneAssignment::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}