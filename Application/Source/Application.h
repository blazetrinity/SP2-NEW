/***********************************************************/
/*!
\file	Application.h
\author	TANG WEN SHENG
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief	Creates an AI Path and storing the Information
*/
/***********************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

/***********************************************************/
/*!
	class CAiPath
\brief	Defines an AI Path and it's method
*/
/***********************************************************/
class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif