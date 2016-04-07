#pragma once 

#include <string> 
#include <vector> 
#include "Player.h"
#include "BoundedBuffer.h"
#include "Achievement.h"

using namespace std;

class AchievementWatcher {
private:
	vector<Achievement*>* achievements;
	Player* player;
	Vector2f playerLastPos;
public:
	AchievementWatcher(vector<Achievement*>* a, Player* p);
	void ApplyAchievement(BoundedBuffer*);
};

