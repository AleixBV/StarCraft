#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "Module.h"

using namespace std;

//Class that manages the game. The win lose cycle, the points earned, the resources, waves etc..
class GameManager : public Module
{
public:
	int currentWaves = 0;
	int totalWaves = 4;
	int score = 0;
	int enemyCount = 0;

	bool isFinished = false;
	bool gameOver = false;
public:

	GameManager()
	{
		
	}


	bool Victory = false;


	bool start();

	bool update();
	



private:


};

#endif