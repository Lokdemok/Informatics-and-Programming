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
	float speedChandegFrames = 0.005f;
public:
	bool isOpen = false;
	String name;
	Sprite sprite;
	bool alive = true;

	Portal(Texture &texture, String Name, Vector2f pos, int W, int H)
		: position(pos)
		, size(W, H)
		, name(Name)
	{
		sprite.setTexture(texture);
		if (name == "yellow")
		{
			sprite.setColor(Color::Yellow);
		}

		sprite.setTextureRect(IntRect(100, 150, size.x, size.y));
	}
	void Update(float time);
	Vector2f GetCoordinates();
	Vector2i GetSize();
	FloatRect GetRect();
};
#endif