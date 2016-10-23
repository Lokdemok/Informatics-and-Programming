#pragma 
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
	void Update(float time)
	{
		currentFrame += time * speedChandegFrames;
		if (currentFrame >= 3)
		{
			currentFrame = 0;
			isOpen = true;
		}
		if (isOpen)
		{
			sprite.setTextureRect(IntRect(100 + w * (int)currentFrame, 150, w, h));
		}
		else
		{
			sprite.setTextureRect(IntRect(344 - w * (int)currentFrame, 150, w, h));
		}
		sprite.setPosition(x - w / 2, y - h / 2);
	}

	Vector2f GetCoordinates(float X, float Y)
	{
		return{ x, y };
	}
	FloatRect GetRect()
	{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, float(w), float(h));//эта ф-ция нужна для проверки столкновений 
	}
};
