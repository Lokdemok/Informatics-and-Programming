#ifndef enemy_h
#define enemy_h
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "level.h"
//#include <iostream>

using namespace sf;


class Enemy {
private:
	Vector2f position;
	Vector2i size;
	Vector2i imagePos;
	Vector2f translocation;
	enum  enemyStates { left, right, stay } state;
	float distanceView;
	bool watchPlayer;
	float shootTimer;
	float speedChangeFrames;
	float timeRecharge = 3000;
public:
	float currentFrame = 0;
	int health;
	int numFrames;
	String name;
	Sprite sprite;
	bool alive;
	bool isAttack;
	bool isShoot;
	int damage;

	Enemy(Texture &texture, String Name, float X, float Y, int W, int H) {
		position = Vector2f(X, Y);
		size = Vector2i(W, H);
		name = Name;
		alive = true;
		speedChangeFrames = float(0.008);
		sprite.setTexture(texture);
		sprite.setOrigin(float(size.x / 2), float(size.y / 2));
		if (name == "easyEnemy")
		{
			imagePos = Vector2i(0, 17);
			health = 2;
			damage = 1;
			translocation.x = -float(0.1);
			numFrames = 9;
		}
		if (name == "flyEnemy")
		{
			imagePos = Vector2i(2, 108);
			health = 5;
			damage = 1;
			state = left;
			distanceView = 1000;
			watchPlayer = false;
			numFrames = 16;
			shootTimer = 0;
			isShoot = false;
		}
		if (name == "trap")
		{
			imagePos = Vector2i(107, 211);
			health = 2;
			damage = 1;
			numFrames = 6;
		}
		sprite.setTextureRect(IntRect(imagePos.x, imagePos.y, size.x, size.y));
	}

	void CheckCollisionWithMap(float Dx, float Dy, std::vector <Object> & objects);
	void Update(float time, Vector2f pos, std::vector <Object> & obj);
	void SetScale(enemyStates newState, Vector2f direction);
	bool CheckWatchPlayer(std::vector<Object>  & obj, Vector2f pos);
	FloatRect GetRect();
};
#endif