#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Creature { 

public:
	float w, h, dx, dy,x,y, speed; 
	enum { left, right, up, down, jump, stay } state;
	int armorScore;
	int health;
	bool life, onGround,isMove;
	String File; 
	Image image;
	Texture texture;
	Sprite sprite;

	Creature(String FileName, float X, float Y, float W, float H) 
	{
		speed = 0; armorScore = 0; health = 100, dx = 0, dy = 0;
		isMove = false; 
		onGround = false;
		life = true;
		File = FileName;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(136, 56, 168));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(1, 101, w, h));
		sprite.setOrigin(w / 2, h / 2);
	}


	void control()
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			state = left;
			speed = 0.1;
	//		CurrentFrame += 0.005*time;
	//		if (CurrentFrame > 10)
	//			CurrentFrame -= 10;
		//	player.sprite.setTextureRect(IntRect(67 * int(CurrentFrame), 521, 67, 70));

		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			state = right;
			speed = 0.1;
		//	CurrentFrame += 0.005*time;
		//	if (CurrentFrame > 10)
		//		CurrentFrame -= 10;
			//player.sprite.setTextureRect(IntRect(67 * int(CurrentFrame), 101, 67, 70));

		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround))
		{
			state = jump; dy = -0.5; onGround = false;
			//CurrentFrame += 0.005*time;
			//if (CurrentFrame > 10)
			//	CurrentFrame -= 10;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			state = down;
			speed = 0.1;
			//CurrentFrame += 0.005*time;
			//if (CurrentFrame > 10)
			//	CurrentFrame -= 10;
		}
	}


	void update(float time) 
	{
		control();
		switch (state)
		{
		case right: dx = speed; break;
		case left: dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case jump: break;
		case stay: break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);
		y += dy*time;
		checkCollisionWithMap(0, dy);
		if (!isMove) speed = 0;
		sprite.setPosition(x + w / 2, y + h / 2); 
		if (health <= 0) { life = false; }
		dy = dy + 0.0015*time; 
	}


	float getcreaturecoordinateX() {	
		return x;
	}
	float getcreaturecoordinateY() {	
		return y;
	}


	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j<(x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')
				{
					if (Dy>0) { y = i * 32 - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = i * 32 + 32;  dy = 0; }
					if (Dx>0) { x = j * 32 - w; }
					if (Dx<0) { x = j * 32 + 32; }
				}
				else { onGround = false; }
			}
	}
};