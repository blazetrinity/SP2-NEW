/***********************************************************/
/*!
\file	AIInteraction.h
\author	Sean Leng
\par
\brief	Adds AI Interaction and storing the Information
*/
/***********************************************************/

#ifndef AIINTERACTION_H
#define AIINTERACTION_H
#include <vector>
#include <string>
#include <Mtx44.h>
#include <algorithm>

using std::random_shuffle;
using std::vector;
using std::string;

/***********************************************************/
/*!
	class CAIInteraction
\brief	Defines an AI Interaction and it's method
*/
/***********************************************************/
class CAIInteraction
{
public:
	CAIInteraction(void);
	~CAIInteraction(void);

	void Set(Mtx44 rotation, Vector3 translation, Vector3 scale);
	void InitAIText();
	void randomText();
	string getText();
	string getPlayerText();

	Mtx44 getRotation();
	Vector3 getTranslation();
	Vector3 getScale();
	float getTime();
	void updateTime(float updatevalue);

private:
	vector<string> PlayerQuestion;
	vector<string> AIText;
	Mtx44 Rotation;
	Vector3 Translation;
	Vector3 Scale;
	Vector3 Offset;
	int value;
	float time;
	string text;
};
#endif
