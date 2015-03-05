/******************************************************************************/
/*!
\file	SoundManager.cpp
\author Vincent Ang
\par	email: Vincent_ang\@nyp.edu.sg
\brief
Manages the Sound for SceneAssignment
*/
/******************************************************************************/

#include "SoundManager.h"

/***********************************************************/
/*!
\brief
	CSoundManager Default Constructor
*/
/***********************************************************/
CSoundManager::CSoundManager(void)
{
	engine = createIrrKlangDevice();
	engine->setDefault3DSoundMinDistance(2.0f);
	engine->setDefault3DSoundMaxDistance(5.0f);
}

/***********************************************************/
/*!
\brief
	CSoundManager Destructor
*/
/***********************************************************/
CSoundManager::~CSoundManager(void)
{
	engine->drop();
}

/***********************************************************/
/*!
\brief
	Adds music for Mall
*/
/***********************************************************/
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

	 engine->setSoundVolume(0.75f);


	return true;

}

/***********************************************************/
/*!
\brief
	Adds music for footstep
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you enter ATM
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you exit ATM
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds Music when you scroll for choices
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you press a button
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when for the Cashier
*/
/***********************************************************/
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
	music->setMaxDistance(15.0f);
	return true;
}

/***********************************************************/
/*!
\brief
	Adds music when you pick up an item
*/
/***********************************************************/
bool CSoundManager::pickItem()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "pick item sound played" << std::endl;

	// play pickup item sound
	ISound* music = engine->play3D("../irrKlang/media/pickup.mp3",
	                              vec3df(0,0,0), false, false, true);

	return true;
}

/***********************************************************/
/*!
\brief
	Adds music when you drop item
*/
/***********************************************************/
bool CSoundManager::dropItem()
{
	if(!engine)
		return false;

	if(engine)
		std::cout << "drop item sound played" << std::endl;

	// play pickup item sound
	ISound* music = engine->play3D("../irrKlang/media/dropItem.mp3",
	                              vec3df(0,0,0), false, false, true);

	return true;
}

/***********************************************************/
/*!
\brief
	Adds music when the liftdoor opens
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when the floor change
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when the gantry door opens
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you give the wrong answer
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you give the correct answer
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you win the game
*/
/***********************************************************/
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

/***********************************************************/
/*!
\brief
	Adds music when you lose the game
*/
/***********************************************************/
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