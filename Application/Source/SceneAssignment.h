/******************************************************************************/
/*!
\file	SceneAssignment.h
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define a Scene
*/
/******************************************************************************/

#ifndef SCENEASSIGNMENT_H
#define SCENEASSIGNMENT_H

#include "MyMath.h"
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
#include "AI.h"
#include "Item.h"
#include "SoundManager.h"
#include <vector>
#include <string>
#include <sstream>
using std::vector;
using std::string;


/******************************************************************************/
/*!
		Class SceneAssignment:
\brief	Defines a Scene and its methods
*/
/******************************************************************************/
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
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHTENABLED,
		
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
	
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	Mesh*meshList[CModel::NUM_GEOMETRY];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Light lights[2];

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh *mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh *mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh *mesh, Color color, float size, float x, float y);
	void RenderObjs();
	void RenderCharacter();
	void RenderTrolley();
	void RenderMenu();
	void RenderAI();
	void RenderCustomerGame();
	void RenderWinScreen();
	void RenderLoseScreen();
	void RenderItemOnCounter();
	void InitSkyboxLevel1();
	void InitLiftLevel1();
	void InitSkyboxLevel2();
	void InitLiftLevel2();
	void InitOBJs();
	void InitCharacterModel();
	void InitAI();
	void InitItemListAndPriceIndex();
	void InitItemsObj();
	void GantryInteraction(double dt);
	void LiftInteraction(double dt);
	void InteractionCheck();
	void Pickup(CSceneObj Object);
	void RemoveTrolley(int i);
	void RenderCashierGame();
	void RandCustomerList();
	void RandShoppingList();
	bool CheckCustomerInventory();
	bool CalTotalPrice(int customerPayingPrice);
	

	void CustomerNavigation();
	void PrintTextInCentre(int start, int end, string arrName[], int highlighted, int StringSize);
	void PrintInventoryBox();
	void UpdateSelectedItem();
	void MenuSelections(int &highlighted, int max, int min);
	void Money(int amount);
	void ATMwithdraw();
	void AtmUpdate();

	string menu[6];
	string navigate[3];
	string cash[12];
	string change[4];

	string amountLeft;

	int box[10];
	int Selected;

	float fps;
	float FloorTimer;
	float MoveDoor;
	float RotateGantry;
	float InteractionTimer;
	float CashierGameKeyPressTimer;
	float CashierGameTimer;
	float CustomerGameTimer;
	float EndTimer;

	float InventRed;
	float InventYellow;
	float InventX;
	float InventY;

	float TranslateItem;

	bool SecurityCamera;
	bool renderCart;

	bool StartGame;
	bool CashierGame;

	bool CustomerGame;

	bool KeyLeft, KeyRight, KeyE, KeyTab, KeyC;

	bool ATMMode;
	bool Ask;
	bool Left;

	bool inventory;
	bool selectItem;


	bool atmEnterSound;
	bool atmExitSound;

	bool liftDoorOpenSound;
	bool liftDoorCloseSound;

	bool gantryDoorOpenSound;
	bool gantryDoorCloseSound;

	bool winScreenSound;
	bool loseScreenSound;

	bool dropItemSound;

	bool winScreen;
	bool loseScreen;



	bool RenderItems;


	int ATMcash;

	int round;
	int chances;

	int CameraMode;

	int highlight;
	int delay;
	int atmHigh;
	int anotherHigh;

	int IndexCounter;

	vector<int> Doorindex;
	int MoveDoorUpperLimit;
	int MoveDoorLowerLimit;
	int diffDistance;
	
	int customerPayingPrice;
	int totalPrice;

	CItem myItem;
	vector<CItem> itemList;
	vector<CItem> shoppingList;

	vector<CModel::GEOMETRY_TYPE> itemRenderList;
	
	vector<string> priceIndex;
	vector<string> customerList;
	
	vector<int> Gantryindex;

	string CustomerGameState;

	CInteraction LiftDoor, Lift, Gantry;

	Ccharacter Character;
	
	Camera3 SecurityCamera1, SecurityCamera2, SecurityCamera3, SecurityCamera4;

	Camera3 PanCam;

	CSoundManager SP_SND;

	CSceneObj myObj;
	vector<CSceneObj> Objs;

	CAi myAI;
	vector<CAi> AiList;

	Vector3 MaximumBound, MinimumBound;
	Vector3 Translate, Scale;
	Vector3 PlayerPosition, CameraPosition, CameraTarget;

	Vector3 ItemRenderLocation;
	Mtx44 Rotate;

	MS modelStack, viewStack, projectionStack;
};

#endif