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
	Vector2u size;
public:
	float currentFrame = 0;
	bool isOpen;
	float x;
	float y;
	int w;
	int h;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	Texture texture;
	bool life;

	Portal(Image &image, String Name, Level &lvl, float X, float Y, int W, int H)
	{
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		texture.loadFromImage(image);
		size = image.getSize();
		sprite.setTexture(texture);
		if (name == "yellow")
		{
			sprite.setColor(Color::Yellow);
		}
		sprite.setTextureRect(IntRect(0, 0, w, h));
		life = true;
		isOpen = false;
	}
	void Update(float time)
	{
		currentFrame += time * 0.005;
		if (currentFrame >= 3)
		{
			currentFrame = 0;
			isOpen = true;
		}
		if (isOpen)
		{
			sprite.setTextureRect(IntRect(w * (int)currentFrame, 0, w, h));
		}
		else
		{
			sprite.setTextureRect(IntRect(size.x - w * (int)currentFrame, 0, w, h));
		}
		sprite.setPosition(x - w / 2, y - h / 2);
	}

	Vector2f GetCoordinates(float X, float Y)
	{
		return{ x, y };
	}
	FloatRect GetRect()
	{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
};
