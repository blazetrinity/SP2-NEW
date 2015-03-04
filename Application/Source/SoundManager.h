#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <iostream>
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

class CSoundManager
{
	ISoundEngine* engine;
public:
	CSoundManager(void);
	~CSoundManager(void);
	
	//Sounds
	bool mallMusic();
	bool ButtonPress();
	bool scrollChoice();
	bool ATMEnter();
	bool ATMExit();
	bool CashierBeep();
	bool pickItem();
	bool footstep();
	bool liftDoor();
	bool gantryDoor();
	bool changeFloor();
	bool cashierGameMusic();
	bool securityGameMusic();
	bool customerGameMusic();
	bool wrongAns();
	bool correctAns();
};

#endif

