#include "AIInteractions.h"


CAIInteraction::CAIInteraction(void)
{
	time = 0.0f;
}


CAIInteraction::~CAIInteraction(void)
{
}

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

void CAIInteraction::Set(Mtx44 rotation, Vector3 translation, Vector3 scale){
	Rotation = rotation;
	Translation = translation;
	Scale = scale;
}

void CAIInteraction::randomText(){
	value = rand() % 9;
	text = AIText[value];
	time = 10.0f;
}


string CAIInteraction::getText(){
	return text;
}

Mtx44 CAIInteraction::getRotation(){
	return Rotation;
}

Vector3 CAIInteraction::getTranslation(){
	return Translation;
}

Vector3 CAIInteraction::getScale(){
	return Scale;
}

float CAIInteraction::getTime()
{
	return time;
}

void CAIInteraction::updateTime(float updatevalue)
{
	time -= updatevalue;
}