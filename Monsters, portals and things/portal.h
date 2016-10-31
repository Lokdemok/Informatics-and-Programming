#ifndef portal_h
#define portal_h
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Player.h"
#include <string>

using namespace sf;


class Portal
{
private:
	Vector2f position;
	Vector2i size;
	float currentFrame = 0;
	float speedChandegFrames;
public:
	bool isOpen;
	String name;
	Sprite sprite;
	bool alive;

	Portal(Texture &texture, String Name, Vector2f pos, int W, int H)
	{
		position = pos;
		size = Vector2i(W, H);
		name = Name;
		speedChandegFrames = float(0.005);
		sprite.setTexture(texture);
		if (name == "yellow")
		{
			sprite.setColor(Color::Yellow);
		}

		sprite.setTextureRect(IntRect(100, 150, size.x, size.y));
		alive = true;
		isOpen = false;
	}
	void Update(float time);
	Vector2f GetCoordinates();
	Vector2i GetSize();
	FloatRect GetRect();
};
#endif