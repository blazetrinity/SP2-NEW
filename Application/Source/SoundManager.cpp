#include "SoundManager.h"

CSoundManager::CSoundManager(void)
{
	engine = createIrrKlangDevice();
	engine->setDefault3DSoundMinDistance(2.0f);
	engine->setDefault3DSoundMaxDistance(5.0f);
}


CSoundManager::~CSoundManager(void)
{
	engine->drop();
}

bool CSoundManager::mallMusic()
{

	 if (!engine)
      return false; // error starting up the engine

	 if (engine)
		 std::cout << "main Music sound played" << std::endl;
	 //play main song
	 ISound* music = engine->play2D("../irrKlang/media/mainMusic.mp3",
	                                true, false, true); 

	// ISound* music = engine->play2D("../irrKlang/media/GameOfThrones.mp3",
	                            //    true, false, true);

	 engine->setSoundVolume(0.5f);


	return true;

}
bool CSoundManager::footstep()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "footstep sound played" << std::endl;

	//play footstep sound
	ISound* music = engine->play2D("../irrKlang/media/footsteps_cut.mp3",
	                               false, false, true);
	
	return true;

}
bool CSoundManager::ATMEnter()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "ATM Enter sound played" << std::endl;

	//play ATM enter sound
	ISound* music = engine->play2D("../irrKlang/media/atm_login.wav",
	                               false, false, true);
	
	return true;

}

bool CSoundManager::ATMExit()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "ATM exit sound played" << std::endl;

	//play ATM exit sound
	ISound* music = engine->play2D("../irrKlang/media/atm_logout.wav",
	                               false, false, true);
	
	return true;

}
bool CSoundManager::scrollChoice()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "Scroll choice sound played" << std::endl;

	//play scroll menu sound
	ISound* music = engine->play2D("../irrKlang/media/atm_Choice.wav",
	                               false, false, true);
	
	return true;
}

bool CSoundManager::ButtonPress()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "button press sound played" << std::endl;

	//play Button press sound
	ISound* music = engine->play2D("../irrKlang/media/atm_Confirm.wav",
	                               false, false, true);
	
	return true;
}

bool CSoundManager::CashierBeep()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "cashier Beep sound played" << std::endl;

	// play cashier scanning sound
	ISound* music = engine->play3D("../irrKlang/media/Cashier_scan.wav",
	                              vec3df(-75,-50,-40), true, false, true);

	//ISound* music2 = engine->play3D("../../media/Cashier_scan.mp3",
	                              //vec3df(-75,-50,60), true, false, true);
	
	//music2->setMaxDistance(25.0f);
	music->setMinDistance(5.0f);
	music->setMaxDistance(25.0f);
	return true;
}

bool CSoundManager::pickItem()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "pick item sound played" << std::endl;

	// play pickup item sound
	ISound* music = engine->play3D("../irrKlang/media/pickup.wav",
	                              vec3df(0,0,0), false, false, true);

	return true;
}

bool CSoundManager::liftDoor()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "lift door sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/liftdoor.wav",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::changeFloor()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "change floor sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/changefloor.wav",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::gantryDoor()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "open gantry door sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/gantrySound.wav",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::wrongAns()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "wrong answer sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/wrong.mp3",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::correctAns()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "correct answer sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/correct.mp3",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::winGame()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "win game sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/win.mp3",
	                               false, false, true);

	
	return true;
}

bool CSoundManager::loseGame()
{
		if(!engine)
		return false;

	if(engine)
		std::cout << "lose game sound played" << std::endl;

	// play lift door sound
	ISound* music = engine->play2D("../irrKlang/media/lose.mp3",
	                               false, false, true);

	
	return true;
}