#pragma once
#include <string> 
#include <vector>  
#include <thread>
#include "SFML\Graphics.hpp"
#include "Player.h"
#include "BoundedBuffer.h"

using namespace std; 

class Monitor {

public:
	Monitor(Player*, BoundedBuffer*);
	void Consumer();
	void MonitorPlayer();
	void DrawMonitorMessages(sf::RenderWindow& window);
private:
	Vector2f lastPlayerPos;
	bool newMessage;
	Player* player;
	BoundedBuffer* buffer;
	std::thread* t1; 
	vector<string> messageText;
	sf::Font font;
	vector<sf::Text*> monitorMessages;
};