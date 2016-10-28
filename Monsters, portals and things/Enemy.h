#ifndef enemy_h
#define enemy_h
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "level.h"
//#include <iostream>

using namespace sf;


class Enemy {
public:
	float dx;
	float dy;
	float currentFrame = 0;
	float x;
	float y;
	int health;
	int w;
	int h;
	int numFrames;
	float distanceView;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	bool life;
	bool watchPlayer;
	bool isAttack;
	bool isShoot;
	int imageX;
	int imageY;
	float shootTimer;
	float speedChangeFrames;
	float timeRecharge = 3000;
	int attack;
	enum  enemyStates { left, right, stay } state;

	Enemy(Texture &texture, String Name, Level &lvl, float X, float Y, int W, int H) {
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		life = true;
		speedChangeFrames = float(0.008);
		obj = lvl.GetAllObjects();
		sprite.setTexture(texture);
		sprite.setOrigin(float(w / 2), float(h / 2));
		if (name == "easyEnemy")
		{
			imageX = 0;
			imageY = 17;
			sprite.setTextureRect(IntRect(imageX, imageY, w, h));
			health = 2;
			attack = 1;
			dx = -float(0.1);
			numFrames = 9;
		}
		if (name == "flyEnemy")
		{
			imageX = 2;
			imageY = 108;
			sprite.setTextureRect(IntRect(imageX, imageY, w, h));
			health = 5;
			attack = 1;
			state = left;
			distanceView = 1000;
			watchPlayer = false;
			numFrames = 16;
		}
		if (name == "trap")
		{
			imageX = 107;
			imageY = 211;
			sprite.setTextureRect(IntRect(imageX, imageY, w, h));
			health = 2;
			attack = 1;
			numFrames = 6;
		}
	}

	void CheckCollisionWithMap(float Dx, float Dy);
	void Update(float time, Vector2f pos);
	void SetScale(enemyStates newState, Vector2f direction);
	bool CheckWatchPlayer(std::vector<Object> obj, Vector2f pos);
	FloatRect GetRect();
};
#endif