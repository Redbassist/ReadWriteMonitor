#include "Player.h"
#pragma once

Player::Player(Vector2f p) : pos(p)
{
	LoadAssets();
}

void Player::LoadAssets()
{
	texture.loadFromFile("circle.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x,texture.getSize().y));
	sprite.setScale(0.2, 0.2);
	sprite.setPosition(pos);
}

void Player::Draw(RenderWindow& window)
{
	sprite.setPosition(pos);
	window.draw(sprite);
}

void Player::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		// left key is pressed: move our character
		pos.x -= 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		// left key is pressed: move our character
		pos.x += 2;
	}
}
