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
	int size;
public:
	float currentFrame = 0;
	bool isOpen;
	float x;
	float y;
	float speedChandegFrames;
	int w;
	int h;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	bool life;

	Portal(Texture &texture, String Name, Level &lvl, float X, float Y, int W, int H)
	{
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		speedChandegFrames = float(0.005);
		size = 281;
		sprite.setTexture(texture);
		if (name == "yellow")
		{
			sprite.setColor(Color::Yellow);
		}
		sprite.setTextureRect(IntRect(100, 150, w, h));
		life = true;
		isOpen = false;
	}
	void Update(float time);
	Vector2f GetCoordinates(float X, float Y);
	FloatRect GetRect();
};
#endif