#ifndef bullet_h
#define bullet_h
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "level.h"

using namespace sf;

class Bullet 
{
public:
	float goalX;
	float goalY;
	float currentFrame = 0;
	float x;
	float y;
	int w;
	int h;
	float numFrames;
	float speed;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	bool life;
	int imageX;
	int imageY;
	float startDX;
	float startDY;
	int attack;

	Bullet(Texture &texture, String Name, Level &lvl, float X, float Y, int W, int H, float xGoal, float yGoal)
	{
		x = X;
		y = Y;
		goalX = xGoal;
		goalY = yGoal;
		life = true;
		w = W;
		h = H;
		name = Name;
		obj = lvl.GetObjects("solid");
		sprite.setTexture(texture);
		sprite.setOrigin(float(w / 2), float(h / 2));
		imageX = 0;
		imageY = 0;
		speed = float(0.3);
		sprite.setTextureRect(IntRect(imageX, imageY, w, h));
		attack = 1;
		startDX = xGoal - X;
		startDY = yGoal - Y;
		numFrames = 7;//даем скорость.этот объект всегда двигается
	}

	void Update(float time);
	FloatRect GetRect();
};
#endif