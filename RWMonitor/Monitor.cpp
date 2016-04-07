#include "Monitor.h"

Monitor::Monitor(Player * p, BoundedBuffer * b)
{
	player = p;
	buffer = b;
	newMessage = false;
	t1 = new std::thread(&Monitor::Consumer, this);
	lastPlayerPos = player->pos;
}

void Monitor::Consumer()
{
	while (true) {
		messageText.push_back(buffer->Fetch());
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void Monitor::MonitorPlayer()
{
	if (player->pos.x < lastPlayerPos.x) {
		buffer->Deposit("Player has moved left!");
	}
	if (player->pos.x > lastPlayerPos.x) {
		buffer->Deposit("Player has moved right!");
	}
	lastPlayerPos = player->pos;
}

void Monitor::DrawMonitorMessages(sf::RenderWindow& window)
{
	if (messageText.size() > 0) { 
		sf::Text* text;
		font.loadFromFile("arial.ttf");
		text = new sf::Text(messageText[0], font, 30);
		text->setPosition(50, 680);
		text->setColor(sf::Color::White);
		monitorMessages.push_back(text);
		messageText.erase(messageText.begin());
	}

	monitorMessages.size();

	for (int i = 0; i < monitorMessages.size(); i++) {
		float newY = monitorMessages[i]->getPosition().y;
		newY -= 1;
		monitorMessages[i]->setPosition(Vector2f(monitorMessages[i]->getPosition().x, newY));
		int a = monitorMessages[i]->getColor().a;
		a -= 1;
		monitorMessages[i]->setColor(sf::Color(255, 255, 255, a));
		if (a <= 0) {
			monitorMessages.erase(monitorMessages.begin() + i);
			i--;
		}
		else {
			window.draw(*monitorMessages[i]);
		}
	}
}
