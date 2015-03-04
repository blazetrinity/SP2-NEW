/******************************************************************************/
/*!
\file	SceneAssignment.cpp
\author Malcolm
\par	email: Malcolm\@nyp.edu.sg
\brief
Define the SceneAssignment and it's method
*/
/******************************************************************************/

#include "SceneAssignment.h"
#include "GL\glew.h"
#include <sstream>

#include "shader.hpp"
#include "Mtx44.h"
#include "LoadTGA.h"
#include "Application.h"
#include "MeshBuilder.h"


SceneAssignment::SELECT SceneAssignment::currentScene = MENU;

/***********************************************************/
/*!
\brief
	SceneAssignment default constructor
*/
/***********************************************************/
SceneAssignment::SceneAssignment()
{
}

/***********************************************************/
/*!
\brief
	SceneAssignment deconstructor
*/
/***********************************************************/
SceneAssignment::~SceneAssignment()
{
}

/***********************************************************/
/*!
\brief
	Initialises the SceneAssignment
*/
/***********************************************************/
void SceneAssignment::Init()
{
	//Init music here
	SP_SND.mallMusic();
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
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	//Light 1
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(1, 1, 1);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 2;
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

	//Light 2
	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(-1, -1, -1);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 2;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

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

	meshList[CModel::GEO_BACKGROUND] = MeshBuilder::GenerateQuad("BackGround",Color(0,0,0), 10, 10);
	meshList[CModel::GEO_BACKGROUND] ->textureID = LoadTGA("Image//BackGround.tga");

	meshList[CModel::GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[CModel::GEO_TEXT] ->textureID = LoadTGA("Image//Fixedsys.tga");
	
	InitSkyboxLevel1();
	InitSkyboxLevel2();
	InitOBJs();
	InitItemsObj();
	InitCharacterModel();
	InitAI();
	InitItemListAndPriceIndex();

	LiftDoor.setLevel(1);
	LiftDoor.setInteractionBound(Vector3(0,-1,-190),Vector3(75,1,-90));

	Lift.setLevel(1);
	Lift.setInteractionBound(Vector3(90,-1,-190),Vector3(140,1,-90));

	Gantry.setLevel(1);
	Gantry.setInteractionBound(Vector3(-130,-1,-20),Vector3(-85,1,20));

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

	highlight = 2;
	atmHigh = 3;
	anotherHigh = 10;
	ATMcash = 1000;
	delay = 0;

	ATMMode = false;
	atmEnterSound = true;
	atmExitSound = true;
	liftDoorOpenSound = true;
	liftDoorCloseSound = true;
	gantryDoorOpenSound = true;
	gantryDoorCloseSound = true;
	winScreenSound = true;
	loseScreenSound = true;

	Ask = false;
	Left = false;

	KeyK = false;

	std::stringstream amount;
	amount << ATMcash;

	cash[0] = "FINGERPRINT";
	cash[1] = "DETACTED";
	cash[2] = "YOU HAVE $" + amount.str();
	cash[3] = "->$10";
	cash[4] = "->$50";
	cash[5] = "->$100";
	cash[6] = "->END";
	cash[7] = "Would you like";
	cash[8] = "to make another";
	cash[9] = "transaction?";
	cash[10] = "->YES";
	cash[11] = "->NO";

	navigate[0] = "Press 'TAB' for Inventory";
	navigate[1] = "Press 'K' to Interact";

	Selected = 0;

	inventory = false;
	selectItem = false;

	change[0] = "FINGERPRINT";
	change[1] = "DETACTED";
	change[2] = "BANK OVERDRAFT";
	change[3] = " ";

	CameraMode = -1;
	SecurityCamera = false;
	fps = 0;
	RotateGantry = 0;
	MoveDoor = 0;
	MoveDoorUpperLimit = 45;
	MoveDoorLowerLimit = 0;
	FloorTimer = 0;
	renderCart = false;
	diffDistance = 5;
	InteractionTimer = 0;
	CustomerGameTimer = 0;
	CustomerGame = false;
	winScreen = false;
	loseScreen = false;
	EndTimer = 0;

	CashierGame = false;
	customerPayingPrice = 0;
	totalPrice = 0;
	round = 1;
	chances = 3;
	CashierGameKeyPressTimer = 0;
	CashierGameTimer = 0;
	StartGame = false;
}

/***********************************************************/
/*!
\brief
	Initialises Level 1 of Skybox
*/
/***********************************************************/
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
	
	
	meshList[CModel::GEO_WANTED_WALL] = MeshBuilder::GenerateQuad("WantedWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_WANTED_WALL]->textureID = LoadTGA("Image//Skybox_Wanted.tga");
	
	//
	MaximumBound.Set(60,50,-190);
	MinimumBound.Set(-60,-50,-210);
	Translate.Set(0,0,-200);
	Scale.Set(100,100,100);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_WANTED_WALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

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
	meshList[CModel::GEO_ENTERANCE]->textureID = LoadTGA("Image//Skybox_Enterance.tga");

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

/***********************************************************/
/*!
\brief
	Initialises level 1 of the lift
*/
/***********************************************************/
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
	
	meshList[CModel::GEO_LIFT_FLOOR1] = MeshBuilder::GenerateQuad("LiftWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_LIFT_FLOOR1]->textureID = LoadTGA("Image//Floor Number 1.tga");

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_FLOOR1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::WALL);

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

/***********************************************************/
/*!
\brief
	Initialises Skybox Level 2
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Initialises Lift Level 2
*/
/***********************************************************/
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
	
	meshList[CModel::GEO_LIFT_FLOOR2] = MeshBuilder::GenerateQuad("LiftWall", Color(0, 0, 0), 1, 1);
	meshList[CModel::GEO_LIFT_FLOOR2]->textureID = LoadTGA("Image//Floor Number 2.tga");

	////Lift Wall Front from inside the lift
	MaximumBound.Set(60,50,-100);
	MinimumBound.Set(40,-50,-160);
	Translate.Set(50,0,-125);
	Scale.Set(50,100,50);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_LIFT_FLOOR2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::WALL);

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

/***********************************************************/
/*!
\brief
	Initialises Item Objects
*/
/***********************************************************/
void SceneAssignment::InitItemsObj()
{
	meshList[CModel::GEO_SODA] = MeshBuilder::GenerateOBJ("SodaCan", "OBJ//OBJs//SodaCan.obj");
	meshList[CModel::GEO_SODA]->textureID = LoadTGA("Image//Items//SodaCan.tga");

	myItem.Set(CModel::GEO_SODA,1,"Soda");
	itemList.push_back(myItem);

	meshList[CModel::GEO_HERSHEYS] = MeshBuilder::GenerateOBJ("HerSheys", "OBJ//OBJs//Hershey.obj");
	meshList[CModel::GEO_HERSHEYS]->textureID = LoadTGA("Image//Items//hershey.tga");

	myItem.Set(CModel::GEO_HERSHEYS, 3, "Hersheys");
	itemList.push_back(myItem);

	meshList[CModel::GEO_GUMMYCEREAL] = MeshBuilder::GenerateOBJ("GummyCereal", "OBJ//OBJs//GummyBearsCereal.obj");
	meshList[CModel::GEO_GUMMYCEREAL]->textureID = LoadTGA("Image//Items//GummyBears.tga");

	myItem.Set(CModel::GEO_GUMMYCEREAL, 4, "GummyCereal");
	itemList.push_back(myItem);

	meshList[CModel::GEO_CEREALKBOX] = MeshBuilder::GenerateOBJ("KCereal", "OBJ//OBJs//CerealKillerCereal.obj");
	meshList[CModel::GEO_CEREALKBOX]->textureID = LoadTGA("Image//Items//CerealKillerCereal.tga");

	myItem.Set(CModel::GEO_CEREALKBOX, 5, "KCereal");
	itemList.push_back(myItem);

	meshList[CModel::GEO_STITCHCEREAL] = MeshBuilder::GenerateOBJ("StitchCereal", "OBJ//OBJs//StitchCereal.obj");
	meshList[CModel::GEO_STITCHCEREAL]->textureID = LoadTGA("Image//Items//stitchCereal.tga");

	myItem.Set(CModel::GEO_STITCHCEREAL, 4, "SitchCereal");
	itemList.push_back(myItem);

	meshList[CModel::GEO_BAKEDCANS] = MeshBuilder::GenerateOBJ("BakedBeans", "OBJ//OBJs//BakedBeans.obj");
	meshList[CModel::GEO_BAKEDCANS]->textureID = LoadTGA("Image//Items//beanCan.tga");

	myItem.Set(CModel::GEO_BAKEDCANS, 3, "BakedBeans");
	itemList.push_back(myItem);

	meshList[CModel::GEO_CHIPCEREAL] = MeshBuilder::GenerateOBJ("ChipCereal", "OBJ//OBJs//ChipMatesCereal.obj");
	meshList[CModel::GEO_CHIPCEREAL]->textureID = LoadTGA("Image//Items//ChipMatesCereal.tga");

	myItem.Set(CModel::GEO_CHIPCEREAL, 5, "ChipCereal");
	itemList.push_back(myItem);

	meshList[CModel::GEO_ORANGE] = MeshBuilder::GenerateOBJ("Oranges", "OBJ//OBJs//Oranges.obj");
	meshList[CModel::GEO_ORANGE]->textureID = LoadTGA("Image//Items//Oranges.tga");

	myItem.Set(CModel::GEO_ORANGE, 1, "Oranges");
	itemList.push_back(myItem);

	meshList[CModel::GEO_FERRERO] = MeshBuilder::GenerateOBJ("Ferrero", "OBJ//OBJs//Ferrero.obj");
	meshList[CModel::GEO_FERRERO]->textureID = LoadTGA("Image//Items//Ferrero.tga");

	myItem.Set(CModel::GEO_FERRERO, 2, "Ferrero");
	itemList.push_back(myItem);

	meshList[CModel::GEO_COKEZERO] = MeshBuilder::GenerateOBJ("CokeZero", "OBJ//OBJs//Coke.obj");
	meshList[CModel::GEO_COKEZERO]->textureID = LoadTGA("Image//Items//Coke.tga");

	myItem.Set(CModel::GEO_COKEZERO, 1, "CokeZero");
	itemList.push_back(myItem);

	meshList[CModel::GEO_MOUNTAINDEW] = MeshBuilder::GenerateOBJ("MountainDew", "OBJ//OBJs//MountainDew.obj");
	meshList[CModel::GEO_MOUNTAINDEW]->textureID = LoadTGA("Image//Items//MountainDew.tga");

	myItem.Set(CModel::GEO_MOUNTAINDEW, 1, "MountainDew");
	itemList.push_back(myItem);

	meshList[CModel::GEO_SPRINGROLLS] = MeshBuilder::GenerateOBJ("SpringRolls", "OBJ//OBJs//SpringRolls.obj");
	meshList[CModel::GEO_SPRINGROLLS]->textureID = LoadTGA("Image//Items//Springrolls.tga");

	myItem.Set(CModel::GEO_SPRINGROLLS, 6, "SpringRolls");
	itemList.push_back(myItem);
}

/***********************************************************/
/*!
\brief
	Initialises OBJs
*/
/***********************************************************/
void SceneAssignment::InitOBJs()
{
	//Inventory boxes
	meshList[CModel::GEO_INVENT_RED] = MeshBuilder::GenerateQuad("Inventory", Color(0, 0, 0), 10, 10);
	meshList[CModel::GEO_INVENT_RED]->textureID = LoadTGA("Image//red.tga");

	meshList[CModel::GEO_INVENT_YELLOW] = MeshBuilder::GenerateQuad("Inventory", Color(0, 0, 0), 10, 10);
	meshList[CModel::GEO_INVENT_YELLOW]->textureID = LoadTGA("Image//yellow.tga");
	
	//Atm Screen
	meshList[CModel::GEO_ATMSCREEN] = MeshBuilder::GenerateQuad("ATM", Color(0, 0, 0), 10, 10);
	meshList[CModel::GEO_ATMSCREEN]->textureID = LoadTGA("Image//ATMScreen.tga");

	////Cashier1
	meshList[CModel::GEO_COUNTER] = MeshBuilder::GenerateOBJ("cashier", "OBJ//Cashier.obj");
	meshList[CModel::GEO_COUNTER]->textureID = LoadTGA("Image//cashierCounter.tga");

	MaximumBound.Set(-42, 50, -20);
	MinimumBound.Set(-115, -50, -82);
	Translate.Set(-80,-50,-40);
	Scale.Set(6,6,6);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_COUNTER, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::COUNTER);

	Objs.push_back(myObj);

	////Cashier2
	meshList[CModel::GEO_COUNTER] = MeshBuilder::GenerateOBJ("cashier", "OBJ//Cashier.obj");
	meshList[CModel::GEO_COUNTER]->textureID = LoadTGA("Image//cashierCounter.tga");

	MaximumBound.Set(-42, 50, 80);
	MinimumBound.Set(-115, -50, 18);
	Translate.Set(-80, -50, 60);
	Scale.Set(6, 6, 6);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_COUNTER, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::COUNTER);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_RIGHTSHELFWSODA] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwSoda.obj");
	meshList[CModel::GEO_RIGHTSHELFWSODA]->textureID = LoadTGA("Image//CanTexture.tga");
	
	MaximumBound.Set(140, 50, -35);
	MinimumBound.Set(115,-50,-105);
	Translate.Set(150,-50,-70);
	Scale.Set(10,10,10);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWSODA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF, CModel::GEO_SODA);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_RIGHTSHELFWCEREAL] = MeshBuilder::GenerateOBJ("Cereal", "OBJ//RightShelfwCereal.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL]->textureID = LoadTGA("Image//cerealBox.tga");

	MaximumBound.Set(140, 50, 25);
	MinimumBound.Set(115, -50, -45);
	Translate.Set(150, -50, -10);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF, CModel::GEO_GUMMYCEREAL);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_RIGHTSHELFWBEANCAN] = MeshBuilder::GenerateOBJ("BeanCan", "OBJ//RightShelfwBeanCan.obj");
	meshList[CModel::GEO_RIGHTSHELFWBEANCAN]->textureID = LoadTGA("Image//beanCan.tga");

	MaximumBound.Set(140, 50, 85);
	MinimumBound.Set(115, -50, 15);
	Translate.Set(150, -50, 50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(-90,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWBEANCAN, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF, CModel::GEO_BAKEDCANS);

	Objs.push_back(myObj);

	////Shelf4
	meshList[CModel::GEO_RIGHTSHELFWCEREAL1] = MeshBuilder::GenerateOBJ("Cereal", "OBJ//RightShelfwCereal1.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL1]->textureID = LoadTGA("Image//cerealBox1.tga");

	MaximumBound.Set(80, 50, 31);
	MinimumBound.Set(0, -50, -1);
	Translate.Set(40, -50, 0);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF, CModel::GEO_CHIPCEREAL);

	Objs.push_back(myObj);

	////Shelf5
	meshList[CModel::GEO_RIGHTSHELFWCEREAL2] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfwCereal2.obj");
	meshList[CModel::GEO_RIGHTSHELFWCEREAL2]->textureID = LoadTGA("Image//displayTable2.tga");

	MaximumBound.Set(80, 50, 1);
	MinimumBound.Set(0, -50, -31);
	Translate.Set(40, -50, 0);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCEREAL2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SHELF, CModel::GEO_CEREALKBOX);

	Objs.push_back(myObj);

	meshList[CModel::GEO_TABLE] = MeshBuilder::GenerateOBJ("CounterTop", "OBJ//counterTop.obj");
	meshList[CModel::GEO_TABLE]->textureID = LoadTGA("Image//CounterTopTexture.tga");

	MaximumBound.Set(-20, 50, -170);
	MinimumBound.Set(-65, -50, -190);
	Translate.Set(-50, -50, -200);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_TABLE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::MONITOR);

	Objs.push_back(myObj);

	meshList[CModel::GEO_MONITOR] = MeshBuilder::GenerateOBJ("cashier", "OBJ//monitor.obj");
	meshList[CModel::GEO_MONITOR]->textureID = LoadTGA("Image//MonitorTexture.tga");

	Translate.Set(-41, -28, -195);
	Scale.Set(20, 20, 20);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_MONITOR, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::MONITOR);

	Objs.push_back(myObj);

	////Trolley1
	meshList[CModel::GEO_TROLLEY] = MeshBuilder::GenerateOBJ("trolley", "OBJ//trolley.obj");
	meshList[CModel::GEO_TROLLEY]->textureID = LoadTGA("Image//trolley.tga");

	MaximumBound.Set(-63, 50, -143);
	MinimumBound.Set(-139, -50, -189);
	Translate.Set(-125, -37, -180);
	Scale.Set(15, 15, 15);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_TROLLEY, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::CART);

	Objs.push_back(myObj);

	////Trolley2

	/*MaximumBound.Set(54, 50, -3);
	MinimumBound.Set(-12, -50, -28);*/
	Translate.Set(-115, -37, -180);
	Scale.Set(15, 15, 15);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_TROLLEY, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::CART);

	Objs.push_back(myObj);

	////Trolley3

	/*MaximumBound.Set(54, 50, -3);
	MinimumBound.Set(-12, -50, -28);*/
	Translate.Set(-105, -37, -180);
	Scale.Set(15, 15, 15);
	Rotate.SetToRotation(180,0,1,0);
	myObj.Set(CModel::GEO_TROLLEY, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::CART);

	Objs.push_back(myObj);

	
	meshList[CModel::GEO_CAMERA] = MeshBuilder::GenerateOBJ("Camera", "OBJ//securityCamera2.obj");
	meshList[CModel::GEO_CAMERA]->textureID = LoadTGA("Image//texture camera.tga");

	//SecurityCamera1
	Translate.Set(SecurityCamera1.position.x, 25, SecurityCamera1.position.z);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_CAMERA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SECURITY_CAM);

	Objs.push_back(myObj);

	//SecurityCamera2
	Translate.Set(SecurityCamera2.position.x-5, 25, SecurityCamera2.position.z-20);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_CAMERA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SECURITY_CAM);

	Objs.push_back(myObj);

	//SecurityCamera3
	Translate.Set(SecurityCamera3.position.x+20, 25, SecurityCamera3.position.z-5);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_CAMERA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SECURITY_CAM);

	Objs.push_back(myObj);

	//SecurityCamera4
	Translate.Set(SecurityCamera4.position.x, 25, SecurityCamera4.position.z+20);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_CAMERA, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::SECURITY_CAM);

	Objs.push_back(myObj);

	meshList[CModel::GEO_GANTRY] = MeshBuilder::GenerateOBJ("Oranges", "OBJ//Gantry.obj");
	meshList[CModel::GEO_GANTRY]->textureID = LoadTGA("Image//GantryTexture.tga");

	MaximumBound.Set(-104, 50, -18);
	MinimumBound.Set(-114,-50,-28);
	Translate.Set(-109, -50, -23);
	Scale.Set(5, 5, 5);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_GANTRY, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::GANTRY);

	Objs.push_back(myObj);

	MaximumBound.Set(-104, 50, 28);
	MinimumBound.Set(-114,-50, 18);
	Translate.Set(-109, -50, 23);
	Scale.Set(5, 5, 5);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_GANTRY, Translate, Scale, Rotate, MinimumBound, MaximumBound, 1, CSceneObj::GANTRY);

	Objs.push_back(myObj);

	////ATM Machine
	meshList[CModel::GEO_ATM] = MeshBuilder::GenerateOBJ("ATM", "OBJ//ATM.obj");
	meshList[CModel::GEO_ATM]->textureID = LoadTGA("Image//ATMTexture.tga");

	MaximumBound.Set(-65, 50, -170);
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

	myObj.Set(CModel::GEO_FOODSTORAGE, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_FERRERO);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_FOODSTORAGE1] = MeshBuilder::GenerateOBJ("foodstorage1", "OBJ//FoodStorage1.obj");
	meshList[CModel::GEO_FOODSTORAGE1]->textureID = LoadTGA("Image//FoodStorage1.tga");

	MaximumBound.Set(-69, 50, 109);
	MinimumBound.Set(-159, -50, 45);
	Translate.Set(-116, -50, 80);
	Scale.Set(7, 8, 7);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_FOODSTORAGE1, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_HERSHEYS);

	Objs.push_back(myObj);

	////Shelf3
	meshList[CModel::GEO_FOODSTORAGE2] = MeshBuilder::GenerateOBJ("foodstorage2", "OBJ//FoodStorage2.obj");
	meshList[CModel::GEO_FOODSTORAGE2]->textureID = LoadTGA("Image//FoodStorage2.tga");

	MaximumBound.Set(140, 50, -49);
	MinimumBound.Set(70, -50, -87);
	Translate.Set(113,-50,-78);
	Scale.Set(7, 8, 7);
	Rotate.SetToRotation(90,0,1,0);

	myObj.Set(CModel::GEO_FOODSTORAGE2, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_SPRINGROLLS);

	Objs.push_back(myObj);

	////Shelf1
	meshList[CModel::GEO_RIGHTSHELFWCANS] = MeshBuilder::GenerateOBJ("cashier", "OBJ//RightShelfWCans.obj");
	meshList[CModel::GEO_RIGHTSHELFWCANS]->textureID = LoadTGA("Image//CanTexture3.tga");

	MaximumBound.Set(139, 50, 31);
	MinimumBound.Set(81, -50, 0);
	Translate.Set(120, -50, -1);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,1,0,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWCANS, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_COKEZERO);

	Objs.push_back(myObj);

	////Shelf2
	meshList[CModel::GEO_RIGHTSHELFWMD] = MeshBuilder::GenerateOBJ("RightShelf", "OBJ//RightShelfwMD.obj");
	meshList[CModel::GEO_RIGHTSHELFWMD]->textureID = LoadTGA("Image//CanTexture2.tga");

	MaximumBound.Set(139,50,0);
	MinimumBound.Set(81,-50,-29);
	Translate.Set(120, -50, 1);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(180,0,1,0);

	myObj.Set(CModel::GEO_RIGHTSHELFWMD, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_MOUNTAINDEW);

	Objs.push_back(myObj);

	////FruitStall
	meshList[CModel::GEO_FRUITSTALL] = MeshBuilder::GenerateOBJ("Oranges", "OBJ//FruitStall.obj");
	meshList[CModel::GEO_FRUITSTALL]->textureID = LoadTGA("Image//FruitStall.tga");

	MaximumBound.Set(-105, 50, -20);
	MinimumBound.Set(-150,-50,-85);
	Translate.Set(-150, -50, -50);
	Scale.Set(10, 10, 10);
	Rotate.SetToRotation(0,0,1,0);

	myObj.Set(CModel::GEO_FRUITSTALL, Translate, Scale, Rotate, MinimumBound, MaximumBound, 2, CSceneObj::SHELF, CModel::GEO_ORANGE);

	Objs.push_back(myObj);

}

/***********************************************************/
/*!
\brief
	Initialises Character Model
*/
/***********************************************************/
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

	meshList[CModel::GEO_CLEANER] = MeshBuilder::GenerateOBJ("Girl's arm", "OBJ//adult_male_body.OBJ");
	meshList[CModel::GEO_CLEANER] ->textureID = LoadTGA("Image//cleaner_uv.tga");

	meshList[CModel::GEO_CLEANERARMS] = MeshBuilder::GenerateOBJ("Girl's arm", "OBJ//arm.OBJ");
	meshList[CModel::GEO_CLEANERARMS] ->textureID = LoadTGA("Image//cleaner_arm.tga");

	meshList[CModel::GEO_GIRL_CLEANER] = MeshBuilder::GenerateOBJ("Girl's arm", "OBJ//adult_female_body.OBJ");
	meshList[CModel::GEO_GIRL_CLEANER] ->textureID = LoadTGA("Image//girl_cleaner_uv.tga");

	meshList[CModel::GEO_GIRL_CLEANERARMS] = MeshBuilder::GenerateOBJ("Girl's arm", "OBJ//arm.OBJ");
	meshList[CModel::GEO_GIRL_CLEANERARMS] ->textureID = LoadTGA("Image//girl_cleaner_arm.tga");
}

/***********************************************************/
/*!
\brief
	Initialise AI
*/
/***********************************************************/
void SceneAssignment::InitAI()
{
	myAI.Set(Vector3(-120,-50,-170), Character.GetPosition(), CModel::GEO_SECURITY, CModel::GEO_SECURITYARM,CAi::STATIONARY,2,7);
	AiList.push_back(myAI);
	
	myAI.Set(Vector3(130, -50, -39), 90, CModel::GEO_GIRL_CLEANER, CModel::GEO_GIRL_CLEANERARMS, CAi::STATIONARY,2,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3(63, -50, 74), 45, CModel::GEO_ADULT3, CModel::GEO_ARM3,CAi::STATIONARY,2,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3(-55, -50, 79), -45, CModel::GEO_ADULT3, CModel::GEO_ARM3,CAi::STATIONARY,2,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3(-121,-50, 3), 0, CModel::GEO_ADULT2, CModel::GEO_ARM2,CAi::STATIONARY,2,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3(-80, -50, 40), 90, CModel::GEO_CASHIER, CModel::GEO_CASHIERARM,CAi::STATIONARY,1,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3( 52, -50, 89), 0, CModel::GEO_CLEANER, CModel::GEO_CLEANERARMS,CAi::STATIONARY,1,7);
	AiList.push_back(myAI);

	myAI.Set(Vector3(-50,-50,-50),180, CModel::GEO_ADULT2, CModel::GEO_ARM2,CAi::MOVING,2,7);
	myAI.AddPath(Vector3(-50,-50,-50));
	myAI.AddPath(Vector3(-50,-50,50));
	myAI.AddPath(Vector3(50,-50,50));
	myAI.AddPath(Vector3(50,-50,-50));
	AiList.push_back(myAI);
}

/***********************************************************/
/*!
\brief
	Initialise Item list and price Index
*/
/***********************************************************/
void SceneAssignment::InitItemListAndPriceIndex()
{
	priceIndex.push_back("Soda - $1");
	priceIndex.push_back("Hersheys- $3");
	priceIndex.push_back("GummyCereal - $4");
	priceIndex.push_back("KCereal- $5");
	priceIndex.push_back("SitchCereal - $4");
	priceIndex.push_back("BakedBeans- $3");
	priceIndex.push_back("ChipCereal- $5");
	priceIndex.push_back("Fruit- $1");
	priceIndex.push_back("Ferrero- $2");
	priceIndex.push_back("CokeZero- $1");
	priceIndex.push_back("MountainDew- $1");
	priceIndex.push_back("ShrimpBox- $6");
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

#define DOORSPEED 40.0f

/***********************************************************/
/*!
\brief
	Updates in SceneAssignment

\param dt
	Using Delta Time to update in SceneAssignment
*/
/***********************************************************/
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
	GantryInteraction(dt);

	//Menu
	if (currentScene == MENU)
	{
		MenuSelections(highlight, 5, 2);

		//selection of scene
		if (Application::IsKeyPressed(VK_RETURN))
		{
			SP_SND.ButtonPress();
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
		
		if (currentScene == EXIT)
		{
			exit(0);
		}
		PanCam.UpdatePan(dt);
	}

	if (currentScene != MENU)
	{
		if(SecurityCamera == false && CashierGame == false && CustomerGame == false)
		{
			if(Application::IsKeyPressed('1'))
			{
				Character.setModel(CModel::GEO_SECURITY,CModel::GEO_SECURITYARM);
				//Character.setModelPosition(7, 0, 9);
				currentScene = SECURITY;
			}

			if(Application::IsKeyPressed('2'))
			{
				Character.setModel(CModel::GEO_CUSTOMER,CModel::GEO_CUSTOMERARM);
				//Character.setModelPosition(-3, 0, 9);
				currentScene = CUSTOMER;
			}

			if(Application::IsKeyPressed('3'))
			{
				Character.setModel(CModel::GEO_CASHIER,CModel::GEO_CASHIERARM);
				//Character.setModelPosition(7, 0, 0);
				currentScene = CASHIER;
			}
		}
			
		Vector3 TempPosition = Character.GetPosition();
		
		if(CashierGame == false && ATMMode == false && inventory == false)
		{
			Character.Update(dt, Objs, AiList);
		}

		else if (ATMMode == true)
		{
			if(atmEnterSound == true)
			{
				SP_SND.ATMEnter();
				atmEnterSound = false;
				atmExitSound = true;
			}
			
			AtmUpdate();
		}

		if(winScreen || loseScreen)
		{
			EndTimer -= 5*dt;
		}

		if(CustomerGame)
		{
			CustomerGameTimer -= 1*dt;
		}

		if(CashierGame)
		{
			CashierGameTimer -= 5*dt;
			CashierGameKeyPressTimer += 5*dt;
		}

		if(CustomerGame == true && CustomerGameTimer < 0)
		{
			CustomerGameState = "Lose";
		}

		KeyLeft = KeyRight = KeyK = KeyTab = false;

		if(CashierGame == true && CashierGameKeyPressTimer > 5)
		{
			if(Application::IsKeyPressed(VK_RIGHT))
			{
				KeyRight = true;
			}

			else if(Application::IsKeyPressed(VK_LEFT))
			{
				KeyLeft = true;
			}
			CashierGameKeyPressTimer = 0;
		}

		if(InteractionTimer > 20 && Application::IsKeyPressed('K'))
		{
			KeyK = true;
			InteractionTimer = 0;
		}

		if(InteractionTimer > 5 &&Application::IsKeyPressed(VK_TAB) && Character.GetModel() == CModel::GEO_CUSTOMER)
		{
			SP_SND.ButtonPress();
			KeyTab = true;
			InteractionTimer = 0;
		}

		InteractionTimer += 10*dt;

		if(KeyK)
		{
			InteractionCheck();
		}

		if(inventory == true)
		{
			UpdateSelectedItem();
		}

		if(KeyTab && inventory == false)
		{
			inventory = true;
		}

		else if(KeyTab && inventory == true)
		{
			inventory = false;
		}

		navigate[2] = "$" + std::to_string(static_cast<long long> (Character.GetWallet()));

		//UpdateAiText
		float TextTimer = 0;
		TextTimer += (float) (2*dt);

		for(int i = 0; i < AiList.size(); ++i)
		{
			if(AiList[i].getAIText().getTime() > 0)
			{
				AiList[i].updateText(TextTimer);
			}
		}

		//UpdateAi
		for(int i = 0; i <AiList.size(); ++i)
		{
			
				if(AiList[i].getAiType() == CAi::STATIONARY && Character.GetPosition() != TempPosition && AiList[i].GetModel() == CModel::GEO_SECURITY)
				{
					AiList[i].CalTarget(Character.GetPosition());
				}
			
				else if(AiList[i].GetModel() == CModel::GEO_SECURITY && AiList[i].getAiType() == CAi::STATIONARY)
				{
					AiList[i].UpDateRotate(dt);
				}

				else if(AiList[i].getAiType() == CAi::MOVING)
				{
					AiList[i].CalMovementAndRotation();
					AiList[i].UpDatePath(dt, Character.getBoundMin(), Character.getBoundMax(), Character.getLevel());
				}

				else if(AiList[i].getAiType() == CAi::STATIONARY && AiList[i].GetModel() != CModel::GEO_SECURITY)
				{
					AiList[i].UpDateAI(dt);
				}
			
		}
	}

	fps = 1/dt;
}

void SceneAssignment::AtmUpdate()
{

	int selection = 0;

	if(Ask == false)
	{
		selection = atmHigh - 1;
	
		//ATM selection
		MenuSelections(atmHigh, 6, 3);
	}

	//prompt another transaction		
	else	
	{
		selection = anotherHigh - 1;
				
		//Yes or No
		MenuSelections(anotherHigh, 11, 10);
	}

	//selected on ATM
	if(Application::IsKeyPressed(VK_RETURN) && delay == 0)
	{
		SP_SND.ButtonPress();
		//get $10
		if(selection == 2)
		{
			if (ATMcash < 10)
			{
				cash[0] = change[2];
				cash[1] = change[3];
			}
		
			else
			{
				Money(10);
				Ask = true;
			}
		}
		
		//get $50
		if (selection == 3)
		{
			if (ATMcash < 50)
			{
				cash[0] = change[2];
				cash[1] = change[3];
			}
		
			else
			{
				Money(50);
				Ask = true;
			}
		}

		//get $100
		if (selection == 4)
		{
			if (ATMcash < 100)
			{
				cash[0] = change[2];
				cash[1] = change[3];
			}
		
			else
			{
				Money(100);
				Ask = true;
			}	
		}
				
		delay = 15;

		//Yes
		if(selection == 9)
		{
			Ask = false;
		}
		// End||No
		if (selection == 5 || selection == 10)
		{
			if(atmExitSound == true)
			{
				SP_SND.ATMExit();
				atmExitSound = false;
				atmEnterSound = true;
			}
			Ask = false;
			ATMMode = false;
			Left = true;
					
			if (Left == true);
			{
				cash[0] = change[0];
				cash[1] = change[1];
			}
		}
			
		if (delay > 0)
		{
			--delay;
		}	

	}
}

void SceneAssignment::UpdateSelectedItem()
{
	if (Application::IsKeyPressed(VK_LEFT) && delay == 0)
	{
		SP_SND.scrollChoice();
		Selected -= 1;
		if(Selected == -1)
		{
			Selected = 9;
		}

		Selected = Selected%10;
		delay = 10;
	}
	if (Application::IsKeyPressed(VK_RIGHT) && delay == 0)
	{
		SP_SND.scrollChoice();
		Selected += 1;
		Selected = Selected%10;
		delay = 10;
	}

	if (delay > 0)
	{
		--delay;
	}

}

/***********************************************************/
/*!
\brief
	Removes the Trolley

\param I
	Using I to count the Index
*/
/***********************************************************/
void SceneAssignment::RemoveTrolley(int i){
	Objs.erase(Objs.begin() + i);
	Character.setInventorySize();
	Character.SetWallet(-1);
}

/***********************************************************/
/*!
\brief
	Checks for interaction in SceneAssignment
*/
/***********************************************************/
void SceneAssignment::InteractionCheck()
{
	for(int i = 0; i < Objs.size(); ++i)
	{
		if(Character.GetCamera().target.x > Objs[i].getBoundMin().x && Character.GetCamera().target.x < Objs[i].getBoundMax().x && Character.GetCamera().target.z > Objs[i].getBoundMin().z && Character.GetCamera().target.z < Objs[i].getBoundMax().z && Character.getLevel() == Objs[i].getLevel())
		{
			//Customer
			if(currentScene == CUSTOMER)
			{
				if(Objs[i].getOBJType() == CSceneObj::SHELF)
				{
					Pickup(Objs[i]);
					for(int n = 0; n < Character.GetInterventory().size(); ++n)
					{
						std::cout << Character.GetInterventory()[n];
					}
					std::cout << std::endl;
				}

				if (Objs[i].getOBJType() == CSceneObj::ATM)
				{
						ATMMode = true;
				}

				if(Objs[i].getOBJType() == CSceneObj::CART)
				{
					if(renderCart == false && Character.GetWallet() > 0)
					{
						RemoveTrolley(i);
						renderCart = true;
						RandShoppingList();
						CustomerGame = true;
						CustomerGameState = "Playing";
						CustomerGameTimer = 300;
						break;
					}
				}
				
				if(Objs[i].getOBJType() == CSceneObj::COUNTER && CustomerGame == true)
				{
					if(CheckCustomerInventory() == true)
					{
						CustomerGameState = "Win";
						Character.ResetInventory();
					}

					else if(CheckCustomerInventory() == false)
					{
						CustomerGameState = "Lose";
						Character.ResetInventory();
					}
				}
			}

			//Cashier	
			else if(currentScene == CASHIER)
			{
				if(Objs[i].getOBJType() == CSceneObj::COUNTER)
				{
					CashierGame = true;
					PlayerPosition = Character.GetPosition();
					CameraPosition = Character.GetCamera().position;
					CameraTarget = Character.GetCamera().target;
					Character.SetCharacterPosCamTar(Vector3(-90,-50,-20),Vector3(-90,-20,-20),Vector3(-90,-20,-10));
					RandCustomerList();
					round = 1;
					chances = 3;
					StartGame = false;
				}
			}

			//Security
			else if(currentScene == SECURITY)
			{

			}
		}	
	}

	//Ai Interaction check
	for(int i = 0; i < AiList.size(); ++i)
	{
		if(Character.GetCamera().target.x > AiList[i].getBoundMin().x && Character.GetCamera().target.x < AiList[i].getBoundMax().x && Character.GetCamera().target.z > AiList[i].getBoundMin().z && Character.GetCamera().target.z < AiList[i].getBoundMax().z && Character.getLevel() == AiList[i].getLevel())
		{
			AiList[i].SetText();
		}	
	}
}

/***********************************************************/
/*!
\brief
	Pickup Items 

\param Object
	Object to be picked up
*/
/***********************************************************/
void SceneAssignment::Pickup(CSceneObj object)
{
	Character.AddToInventory(object.getItem());
}

/***********************************************************/
/*!
\brief
	Lift Interaction

\param dt
	Using Delta Time to update the Lift Interaction
*/
/***********************************************************/
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

			if(liftDoorOpenSound == true)
			{
				SP_SND.liftDoor();
				liftDoorOpenSound = false;
				liftDoorCloseSound = true;
			}
		}
	}

	else
	{
		
		if(MoveDoor > MoveDoorLowerLimit)
		{
			MoveDoor -= (float)(DOORSPEED * dt);
			Objs[Doorindex[0]].setTranslate(50,-50,-175+MoveDoor);
			Objs[Doorindex[1]].setTranslate(50,-50,-175+MoveDoor);
			
			if(liftDoorCloseSound == true)
			{
				SP_SND.liftDoor();
				liftDoorCloseSound = false;
				liftDoorOpenSound = true;
			}
		}
	}

	FloorTimer += 10*dt;

	if(Application::IsKeyPressed('F') && Character.GetPosition().x > Lift.getMinBound().x && Character.GetPosition().x < Lift.getMaxBound().x && Character.GetPosition().z > Lift.getMinBound().z && Character.GetPosition().z < Lift.getMaxBound().z && FloorTimer > 20)
	{
		SP_SND.changeFloor();
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

void SceneAssignment::GantryInteraction(double dt)
{
	if(Gantryindex.size() == 0)
	{
		for(int i = 0; i < Objs.size(); ++i)
		{
			if(Objs[i].getModel() == CModel::GEO_GANTRY)
			{	
				Gantryindex.push_back(i);	
			}
		}
	}

	if(Character.GetPosition().x > Gantry.getMinBound().x && Character.GetPosition().x < Gantry.getMaxBound().x && Character.GetPosition().z > Gantry.getMinBound().z && Character.GetPosition().z < Gantry.getMaxBound().z)
	{
		if(RotateGantry < 90)
		{
			RotateGantry += (float)(40 *dt);
			Objs[Gantryindex[0]].setRotate(0-RotateGantry,0,1,0);
			Objs[Gantryindex[1]].setRotate(180+RotateGantry,0,1,0);

			if(gantryDoorOpenSound == true)
			{
				SP_SND.gantryDoor();
				gantryDoorOpenSound = false;
				gantryDoorCloseSound = true;
			}
		}
	}

	else
	{
		if(RotateGantry > 0)
		{
			RotateGantry -= (float)(40 *dt);
			Objs[Gantryindex[0]].setRotate(0-RotateGantry,0,1,0);
			Objs[Gantryindex[1]].setRotate(180+RotateGantry,0,1,0);

			if(gantryDoorCloseSound == true)
			{
				SP_SND.gantryDoor();
				gantryDoorCloseSound = false;
				gantryDoorOpenSound = true;
			}
		}
	}
}

void SceneAssignment::MenuSelections(int &highlighted, int max, int min)
{
	if (Application::IsKeyPressed(VK_DOWN) && delay == 0 && highlighted < max)
	{
		SP_SND.scrollChoice();
		highlighted += 1;
		delay = 15;
	}
	if (Application::IsKeyPressed(VK_UP) && delay == 0 && highlighted > min)
	{
		SP_SND.scrollChoice();
		highlighted -= 1;
		delay = 15;
	}

	if (delay > 0)
	{
		--delay;
	}
}

/***********************************************************/
/*!
\brief
	Render Mesh in SceneAssignment

\param Mesh
	Adds the Mesh

\param enableLight
	Enable the Lights
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Render in SceneAssignment
*/
/***********************************************************/
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

	//Light 2
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[CModel::GEO_AXES], false);

	RenderObjs();

	if (currentScene == MENU)
	{
		//RenderMenu
		RenderMenu();
	}
	else
	{
		RenderCharacter();
		RenderAI();
		
		if(currentScene == CASHIER)
		{
			if(CashierGame)
			{
				RenderCashierGame();
			}
		}

		if(currentScene == CUSTOMER)
		{
			if(CustomerGame == false)
			{
				CustomerNavigation();
			}

			if(renderCart)
			{
				RenderTrolley();
			}

			if(inventory)
			{
				PrintInventoryBox();
			}

			if(CustomerGame && inventory == false)
			{
				RenderCustomerGame();
			}

			if(ATMMode)
			{
				//Render ATM background
				RenderImageOnScreen(meshList[CModel::GEO_ATMSCREEN], Color(0,0,0), 80.f, 0.5f, 0.4f);

				if (Ask == false)
				{
					//Render ATM Screen
					PrintTextInCentre(0, 7, cash, atmHigh, 7);
				}

				else
				{
					//Render Prompt Screen
					PrintTextInCentre(7, 12, cash, anotherHigh, 18); 
				}
			}
		}
	}

	if(winScreen)
	{
		RenderWinScreen();
	}

	if(loseScreen)
	{
		RenderLoseScreen();
	}

	
	std::ostringstream stringfps;

	stringfps << fps;

	RenderTextOnScreen(meshList[CModel::GEO_TEXT], "FPS:", Color(0, 1, 0), 2, 1, 1);

	RenderTextOnScreen(meshList[CModel::GEO_TEXT], stringfps.str(), Color(0, 1, 0), 2, 5, 1);
}

void SceneAssignment::RenderWinScreen()
{
	RenderTextOnScreen(meshList[CModel::GEO_TEXT], "YOU WIN!", Color(0, 1, 0), 3, 10, 10);

	if(winScreenSound == true)
	{
		SP_SND.winGame();
		winScreenSound = false;
	}

	if(EndTimer < 0)
	{
		winScreen = false;
	}
}

void SceneAssignment::RenderLoseScreen()
{
	RenderTextOnScreen(meshList[CModel::GEO_TEXT], "YOU LOSE", Color(0, 1, 0), 3, 10, 10);

	if(loseScreenSound == true)
	{
		SP_SND.loseGame();
		loseScreenSound = false;
	}

	if(EndTimer < 0)
	{
		loseScreen = false;
	}
}

/***********************************************************/
/*!
\brief
	Calculates the Total Price of the Item

\param customerPayingPrice
	Use the Customer Paying Price to Calculate

*/
/***********************************************************/
bool SceneAssignment::CalTotalPrice(int customerPayingPrice)
{
	totalPrice = 0;

	for(int i = 0; i < customerList.size(); ++i)
	{
		for(int j = 0; j < itemList.size(); ++j)
		{
			if(customerList[i] == itemList[j].GetItemName())
			{
				totalPrice += itemList[j].GetPrice();
			}
		}
	}

	if(totalPrice <= customerPayingPrice)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool SceneAssignment::CheckCustomerInventory()
{
	vector<bool> booleanCheck;

	if(Character.GetInterventory().size() != shoppingList.size())
	{
		return false;
	}

	for(int n = 0; n < shoppingList.size(); ++n)
	{
		for(int m = 0; m < Character.GetInterventory().size(); ++m)
		{
			if(shoppingList[n].GetModel() == Character.GetInterventory()[m])
			{
				booleanCheck.push_back(true);
				Character.RemoveFromInventory(m);
				m = Character.GetInterventory().size();
			}

			else if(m == Character.GetInterventory().size()-1)
			{
				booleanCheck.push_back(false);
			}
		}
	}

	for(int j = 0; j < booleanCheck.size(); ++j)
	{
		if(booleanCheck[j] == false)
		{
			return false;
		}

		else if(j == booleanCheck.size()-1)
		{
			return true;
		}
	}
}

/***********************************************************/
/*!
\brief
	Randomises Customer List
*/
/***********************************************************/
void SceneAssignment::RandCustomerList()
{
	customerList.clear();

	for(int i = 0; i < 10; ++i)
	{
		int value = rand() % 11;
		customerList.push_back(itemList[value].GetItemName());
	}
	
	int value2 = rand() % 70 + 7;
	customerPayingPrice = value2;

	CashierGameTimer = 100;
}

void SceneAssignment::RandShoppingList()
{
	shoppingList.clear();

	Character.ResetInventory();

	for(int i = 0; i < 10; ++i)
	{
		int value = rand() % 11;
		shoppingList.push_back(itemList[value]);
	}
}

/***********************************************************/
/*!
\brief
	Render Cashier Game
*/
/***********************************************************/
void SceneAssignment::RenderCashierGame()
{
	RenderImageOnScreen(meshList[CModel::GEO_BACKGROUND], Color(0,0,0), 80.f, 0.5f, 0.4f);

	if(StartGame == true)
	{
		if(round < 11 && chances > 0)
		{
			int x1 = 1;
			int x2 = 25;
			int y = 28;

			for(int i = 0; i < priceIndex.size(); ++i)
			{
				RenderTextOnScreen(meshList[CModel::GEO_TEXT], priceIndex[i], Color(1, 1, 1), 2, x1, y - (2*i));
			}

			for(int i = 0; i < customerList.size(); ++i)
			{
				RenderTextOnScreen(meshList[CModel::GEO_TEXT], customerList[i], Color(1, 1, 1), 2, x2, y - (2*i));
			}

			std::ostringstream stringfps;

			stringfps << CashierGameTimer;

			RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Customer Offers: ", Color(1, 1, 1), 2, 20, 1);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], std::to_string(static_cast <long double> (customerPayingPrice)), Color(1, 1, 1), 2, 37, 1);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Life: ", Color(1, 1, 1), 2, 20, 2);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], std::to_string(static_cast <long double> (chances)), Color(1, 1, 1), 2, 26, 2);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Round: ", Color(1, 1, 1), 2, 29, 2);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], std::to_string(static_cast <long double> (round)), Color(1, 1, 1), 2, 36, 2);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Round Timer: ", Color(1, 1, 1), 2, 1, 3);
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], stringfps.str(), Color(1, 1, 1), 2, 1, 2);

			if(CashierGameTimer < 0)
			{
				round++;
				chances--;
			
				RandCustomerList();
			}

			if(KeyRight)
			{
				if(CalTotalPrice(customerPayingPrice) == true)
				{
					SP_SND.correctAns();
					round++;
				}
	
				else if(CalTotalPrice(customerPayingPrice) == false)
				{
					SP_SND.wrongAns();
					round++;
					chances--;
				}
				RandCustomerList();
			}

			if(KeyLeft)
			{
				if(CalTotalPrice(customerPayingPrice) == true)
				{
					SP_SND.wrongAns();
					round++;
					chances--;
				}

				else if(CalTotalPrice(customerPayingPrice) == false)
				{
					SP_SND.correctAns();
					round++;
				}
				RandCustomerList();
			}
		}

		else if(round > 10 && chances > 0)
		{
			winScreenSound = true;
			CashierGame = false;
			winScreen = true;
			EndTimer = 40;
			Character.SetCharacterPosCamTar(PlayerPosition,CameraPosition,CameraTarget);
		}

		else if(chances <= 0)
		{
			loseScreenSound = true;
			CashierGame = false;
			loseScreen = true;
			EndTimer = 40;
			Character.SetCharacterPosCamTar(PlayerPosition,CameraPosition,CameraTarget);
		}
	}

	else if(StartGame == false)
	{
		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Cashier Game", Color(1, 1, 1), 3, 8, 15);
		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Press Right Arrow to accept payment.", Color(1, 1, 1), 2, 3, 9);
		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Press Left Arrow to refuse payment.", Color(1, 1, 1), 2, 3, 8);
		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Press Space to begin.", Color(1, 1, 1), 2, 12, 7);

		if(Application::IsKeyPressed(VK_SPACE))
		{
			StartGame = true;
			CashierGameTimer = 100;
		}
	}
}

void SceneAssignment::RenderCustomerGame()
{
	if(CustomerGameState == "Playing")
	{
		int x = 1;
		int y = 26;
	
		//RenderImageOnScreen(meshList[CModel::GEO_BACKGROUND], Color(0,0,0), 40.f, 0.5f, 0.4f);

		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Shopping List:", Color(0, 0, 0), 2, x, 28);
	
		for(int i = 0; i < shoppingList.size() ; ++i)
		{
			RenderTextOnScreen(meshList[CModel::GEO_TEXT], shoppingList[i].GetItemName(), Color(0, 0, 0), 2, x, y - (2*i));
		}

		std::ostringstream stringfps;

		stringfps << CustomerGameTimer;

		RenderTextOnScreen(meshList[CModel::GEO_TEXT], "Game Timer: ", Color(0, 0, 0), 2, 1, 4);
		RenderTextOnScreen(meshList[CModel::GEO_TEXT], stringfps.str(), Color(0, 0, 0), 2, 1, 3);
	}

	else if(CustomerGameState == "Win")
	{
		winScreenSound = true;
		winScreen = true;
		EndTimer = 40;
		CustomerGame = false;
	}

	else if(CustomerGameState == "Lose")
	{
		loseScreenSound = true;
		loseScreen = true;
		EndTimer = 40;
		CustomerGame = false;
	}
}

/***********************************************************/
/*!
\brief
	Render AI in Scene Assignment
*/
/***********************************************************/
void SceneAssignment::RenderAI()
{
	for(int i = 0; i < AiList.size(); ++i)
	{
		if(AiList[i].getLevel() == Character.getLevel())
		{
			modelStack.PushMatrix();
			modelStack.Translate(AiList[i].GetPosition().x, AiList[i].GetPosition().y,	AiList[i].GetPosition().z);
			modelStack.MultMatrix(AiList[i].GetRotation());
			modelStack.Scale(AiList[i].GetScale().x,AiList[i].GetScale().y,AiList[i].GetScale().z);
			RenderMesh(meshList[AiList[i].GetModel()], true);
			{
				modelStack.PushMatrix();
				{
					modelStack.PushMatrix();
					modelStack.Translate(-1.3, 0, 0);
					modelStack.Rotate(-90, 0, 1, 0);
					RenderMesh(meshList[AiList[i].GetModelArm()], true);
					modelStack.PopMatrix();
				}
				modelStack.Translate(1.3, 0, 0);
				modelStack.Rotate(90, 0, 1, 0);
				RenderMesh(meshList[AiList[i].GetModelArm()], true);
				modelStack.PopMatrix();
			}
	
			modelStack.PopMatrix();

			if(AiList[i].getAIText().getTime() > 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate(AiList[i].getAIText().getTranslation().x, AiList[i].getAIText().getTranslation().y, AiList[i].getAIText().getTranslation().z);
				modelStack.MultMatrix(AiList[i].getAIText().getRotation());
				modelStack.Scale(AiList[i].getAIText().getScale().x, AiList[i].getAIText().getScale().y, AiList[i].getAIText().getScale().z);
				RenderText(meshList[CModel::GEO_TEXT], AiList[i].getAIText().getText(), Color(0, 1, 0));
				modelStack.PopMatrix();
			}
		}
	}
}

/***********************************************************/
/*!
\brief
	Render Trolley in Scene Assignment
*/
/***********************************************************/
void SceneAssignment::RenderTrolley()
{
	trolleyPos.Set(0,0,0);
	DirectionVector = Character.GetCamera().target - Character.GetPosition();
	DirectionVector.Normalize();
	trolleyPos = (DirectionVector * diffDistance) + Character.GetPosition();

	for(int a = 0; a < Objs.size(); ++a)
	{
		if(Objs[a].getOBJType() == CSceneObj::CART)
		{
			modelStack.PushMatrix();
			modelStack.Translate(trolleyPos.x, trolleyPos.y + 10, trolleyPos.z);
			modelStack.Rotate(Character.GetRotation() + 90, 0, 1, 0);
			modelStack.Translate(35, 0, 0);
			modelStack.Scale(Objs[a].getScale().x, Objs[a].getScale().y, Objs[a].getScale().z);
			RenderMesh(meshList[Objs[a].getModel()], true);
			modelStack.PopMatrix();
			break;
		}
	}
}

/***********************************************************/
/*!
\brief
	Render Character in Scene Assignment
*/
/***********************************************************/
void SceneAssignment::RenderCharacter()
{
	modelStack.PushMatrix();
	modelStack.Translate(Character.GetPosition().x,Character.GetPosition().y,Character.GetPosition().z);
	modelStack.Rotate(Character.GetRotation(),0,1,0);
	modelStack.Scale(Character.GetScale().x,Character.GetScale().y,Character.GetScale().z);
	RenderMesh(meshList[Character.GetModel()], true);
	{
		modelStack.PushMatrix();
		{
			modelStack.PushMatrix();
			modelStack.Translate(-1.3, 0, 0);
			modelStack.Rotate(-90, 0, 1, 0);
			RenderMesh(meshList[Character.GetModelArm()], true);
			modelStack.PopMatrix();
		}
		modelStack.Translate(1.3, 0, 0);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[Character.GetModelArm()], true);
		modelStack.PopMatrix();
	}
	
	modelStack.PopMatrix();
}

/***********************************************************/
/*!
\brief
	Render Menu in SceneAssignment
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Render Objects in SceneAssignment
*/
/***********************************************************/
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
			RenderMesh(meshList[Objs[i].getModel()],true);
			modelStack.PopMatrix();
		}
	}
}

void SceneAssignment::PrintTextInCentre(int start, int end, string arrName[], int highlighted, int StringSize)
{
	int colour = 1;
	//menu
	for (int text = start; text < end; text++)
	{
		const float TextSize = 3;
		int x = 80/TextSize/2 - arrName[text].length()/2;
		int y = 60/TextSize/2 + StringSize/2 - text;

		if(highlighted == text)
		{
			colour = 0;
		}

		RenderTextOnScreen(meshList[CModel::GEO_TEXT], arrName[text], Color(1, colour, colour), TextSize, x, y);

		if(highlighted == text)
		{
			colour = 1;
		}
	}
}

void SceneAssignment::Money(int amount)
{
	Character.SetWallet(amount);
	ATMcash -= amount;
	
	std::stringstream amountLeft;
	amountLeft << ATMcash;
	cash[2] = "YOU HAVE $" + amountLeft.str();
}

void SceneAssignment::CustomerNavigation()
{
	float textSize = 2.5f;
	float topLeftHeight = 80.f/(textSize*1.5);
	float topLeftStart = 0.5f;

	for (int text = 0; text < 3; text++)
	{
	RenderTextOnScreen(meshList[CModel::GEO_TEXT], navigate[text], Color(0, 0, 1), textSize, topLeftStart, topLeftHeight - (text - 1));
	}
}

void SceneAssignment::PrintInventoryBox()
{
	float BoxSize = 80/7;
	float x = (BoxSize/2.f - BoxSize/2.5f);
	float y = 60.f/BoxSize/1.5f;
	int count = 0;

	for (int row = 0; row < 2; row++)
	{	
		for (int column = 0; column < 5; column ++)
		{
			if(count == Selected)
			{
				selectItem = true;
				RenderImageOnScreen(meshList[CModel::GEO_INVENT_RED], Color(0,0,0), BoxSize, x + (column * 1.2f), y - (row * 1.2f));	
			}
			
			else
			{
				selectItem = false;
				RenderImageOnScreen(meshList[CModel::GEO_INVENT_YELLOW], Color(0,0,0), BoxSize, x + (column * 1.2f), y - (row * 1.2f));
			}

			
			count++;
		}
	}
}

/***********************************************************/
/*!
\brief
	Render Text in Scene Assignment
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Render Text on Screen
*/
/***********************************************************/
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

void SceneAssignment::RenderImageOnScreen(Mesh *mesh, Color color, float size, float x, float y)
{
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

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

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

/***********************************************************/
/*!
\brief
	Exit Scene Assignment
*/
/***********************************************************/

void SceneAssignment::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}