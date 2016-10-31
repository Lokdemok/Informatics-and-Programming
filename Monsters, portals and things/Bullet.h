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
	Vector2i imagePosition = Vector2i(0, 0);;
	Vector2f translocation;
public:
	float numFrames = 7;
	float speed = 0.3f;
	String name;
	Sprite sprite;
	bool alive = true;
	int damage = 1;

	Bullet(Texture &texture, String Name, float X, float Y, int W, int H, Vector2f goalPos)
		: position(X, Y)
		, goal(goalPos)
		, size(W, H)
		, name(Name)
	{
		translocation = goal - position;
		sprite.setTexture(texture);
		sprite.setOrigin(float(size.x / 2), float(size.y / 2));
		sprite.setTextureRect(IntRect(imagePosition.x, imagePosition.y, size.x, size.y));
	}
	void Update(float time, std::vector <Object> & obj);
	FloatRect GetRect();
};
#endif