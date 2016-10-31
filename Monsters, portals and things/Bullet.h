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
private:
	Vector2f position;
	Vector2i size;
	float currentFrame = 0;
	Vector2f goal;
	Vector2f imagePosition;
	Vector2f translocation;
public:
	float numFrames;
	float speed;
	String name;
	Sprite sprite;
	bool alive;
	int damage;

	Bullet(Texture &texture, String Name, float X, float Y, int W, int H, Vector2f goalPos)
	{
		position = Vector2f(X, Y);
		goal = goalPos;
		alive = true;
		size = Vector2i(W, H);
		name = Name;
		speed = float(0.3);
		damage = 1;
		translocation = goal - position;
		numFrames = 7;
		sprite.setTexture(texture);
		sprite.setOrigin(float(size.x / 2), float(size.y / 2));
		imagePosition = Vector2f(0, 0);
		sprite.setTextureRect(IntRect(imagePosition.x, imagePosition.y, size.x, size.y));
	}
	void Update(float time, std::vector <Object> & obj);
	FloatRect GetRect();
};
#endif