#pragma once 

#include "SFML/Graphics.hpp"
#include <string>

using namespace sf;
using namespace std;

class Player {
private: 
	Texture texture;
	Sprite sprite;
public:
	string name = "Matthew";
	Vector2f pos;
	Player(Vector2f);
	void LoadAssets();
	void Draw(RenderWindow& window);
	void Update();
};
 
