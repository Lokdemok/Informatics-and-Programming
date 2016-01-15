#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Creature { 
public:
	float x, y, w, h, dx, dy, speed = 0; 
	int dir = 0; //направление (direction)
	String File; 
	Image image;
	Texture texture;
	Sprite head;

	Creature(String FileName, float X, float Y, float W, float H) {
		File = FileName;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(136, 56, 168));
		texture.loadFromImage(image);
		head.setTexture(texture);
		x = X; y = Y;
		head.setTextureRect(IntRect(0, 0, w, h));
	}



	void update(float time) 
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;
		case 1: dx = -speed; dy = 0;   break;
		case 2: dx = 0; dy = speed;   break;
		case 3: dx = 0; dy = -speed;   break;
		}

		x += dx*time;
		y += dy*time;

		speed = 0;
		head.setPosition(x, y);
	}
}; 