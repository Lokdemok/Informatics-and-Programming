#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Creature { 
private: float x, y;
public:
	float w, h, dx, dy, speed; 
	int dir;
	int armorScore;//направление (direction)
	int health;
	bool life;
	String File; 
	Image image;
	Texture texture;
	Sprite sprite;

	Creature(String FileName, float X, float Y, float W, float H) {
		dir = 0; speed = 0; armorScore = 0; health = 100;
		life = true;
		File = FileName;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(136, 56, 168));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(x, y, w, h));
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
		sprite.setPosition(x, y);
		interactionWithMap();
		if (health <= 0) 
		{
			life = false; 
			speed = 0;
		}
	}


	float getcreaturecoordinateX() {	
		return x;
	}
	float getcreaturecoordinateY() {	
		return y;
	}
	void interactionWithMap()
	{

		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j < (x + w) / 32; j++)//проход по всем квадратам карты+перс может стоять на нескольких квадратах
			{
				if (TileMap[i][j] == '0' || TileMap[i][j] == 's')
				{
					if (dy>0)
					{
						y = i * 32 - h;
					}
					if (dy<0)
					{
						y = i * 32 + 32;
					}
					if (dx>0)
					{
						x = j * 32 - w;
					}
					if (dx < 0)
					{
						x = j * 32 + 32;
					}
				}
				if (TileMap[i][j] == 'f') 
				{
					health -= 40;//если взяли ядовитейший в мире цветок,то переменная health=health-40;
					TileMap[i][j] = ' ';//убрали цветок
				}

				if (TileMap[i][j] == 'h') 
				{
					health += 20;//если взяли сердечко,то переменная health=health+20;
					TileMap[i][j] = ' ';//убрали сердечко
				}

			}
	}
}; 