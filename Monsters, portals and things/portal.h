#pragma 
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace sf;

class Portal
{
public:
	float x, y, moveTimer;
	int w, h;
	bool left, right;
	Texture texture;
	Sprite sprite;
	String name;
	Portal(Image &image, String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	void update(float time)
	{
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};