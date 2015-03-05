/******************************************************************************/
/*!
\file	AIInteraction.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Set and Render AI
*/
/******************************************************************************/

#include "AIInteractions.h"

/******************************************************************************/
/*!
\brief
CAIInteraction default constructor
*/
/******************************************************************************/
CAIInteraction::CAIInteraction(void)
{
	time = 0.0f;
	Offset.Set(0,0,0);;
}

/******************************************************************************/
/*!
\brief
CAIInteraction deconstructor
*/
/******************************************************************************/
CAIInteraction::~CAIInteraction(void)
{
}

/******************************************************************************/
/*!
\brief
Initialises the AI Text
*/
/******************************************************************************/
void CAIInteraction::InitAIText()
{
	AIText.push_back("How Are You?");
	AIText.push_back("Hello");
	AIText.push_back("Oh Apples are Cheap!!!");
	AIText.push_back("This Supermarket is Awesome!");
	AIText.push_back("I am God!");
	AIText.push_back("Food is Good!");
	AIText.push_back("Food is Real!");
	AIText.push_back("I'm Hungry!!!");
	AIText.push_back("Food! Food! Where are you?");
	AIText.push_back("I am buying all of them!!!!!");
}

/******************************************************************************/
/*!
\brief
Sets the rotation, translation and scale value for the AI
*/
/******************************************************************************/
void CAIInteraction::Set(Mtx44 rotation, Vector3 translation, Vector3 scale){
	Rotation = rotation;
	Translation = translation;
	Scale = scale;
}

/******************************************************************************/
/*!
\brief
Randomises the Text that is being rendered
*/
/******************************************************************************/
void CAIInteraction::randomText(){
	Translation -= Offset;
	value = rand() % 9;
	text = AIText[value];
	time = 10.0f;
	int offset = ((text.length()/2)*Scale.x);
	Offset.Set(-offset,0,0);
	Offset = Rotation * Offset;
	Translation += Offset;
}

/******************************************************************************/
/*!
\brief
Gets the Text 

\return
	Returns the Time
*/
/******************************************************************************/
string CAIInteraction::getText(){
	return text;
}
/******************************************************************************/
/*!
\brief
Gets the Rotation for the AI interaction

\return
	Returns the Rotation value
*/
/******************************************************************************/
Mtx44 CAIInteraction::getRotation(){
	return Rotation;
}

/******************************************************************************/
/*!
\brief
Gets the Translation for the AI Interaction

\return
	Returns the Translation value
*/
/******************************************************************************/
Vector3 CAIInteraction::getTranslation(){
	return Translation;
}

/******************************************************************************/
/*!
\brief
Gets the Scale for the AI Interaction

\return
	Returns the Scale value
*/
/******************************************************************************/
Vector3 CAIInteraction::getScale(){
	return Scale;
}

/******************************************************************************/
/*!
\brief
Gets the time for the AI Interaction

\return
	Returns the time
*/
/******************************************************************************/
float CAIInteraction::getTime()
{
	return time;
}

/******************************************************************************/
/*!
\brief
Updates the time

\param updateValue
	Updates the Value for the time
*/
/******************************************************************************/
void CAIInteraction::updateTime(float updatevalue)
{
	time -= updatevalue;
}