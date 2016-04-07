#include "AchievementWatcher.h"

AchievementWatcher::AchievementWatcher(vector<Achievement*>* a, Player* p)
{
	achievements = a;
	player = p;
	playerLastPos = player->pos;
}

void AchievementWatcher::ApplyAchievement(BoundedBuffer* b)
{  
	if (player->pos.x < playerLastPos.x) {
		achievements->at(0)->CompletedAchievement(player->name, b);
		//b->Deposit("Achievement: Walk left achieved!");
	}
	if (player->pos.x > playerLastPos.x) {
		achievements->at(1)->CompletedAchievement(player->name, b);
		//b->Deposit("Achievement: Walk right achieved!");
	}
	if (player->pos.y > playerLastPos.y) {
		achievements->at(2)->CompletedAchievement(player->name, b);
		//b->Deposit("Achievement: Jump achieved!");
	}
	playerLastPos = player->pos;
}
