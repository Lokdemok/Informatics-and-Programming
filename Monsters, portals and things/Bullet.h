#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>

using namespace sf;

class Bullet 
{
public:
	float goalX;
	float goalY;
	float dx;
	float dy;
	float currentFrame = 0;
	float x;
	float y;
	int w;
	int h;
	float numFrames;
	float distanceView;
	float speed;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	Texture texture;
	bool life;
	int spriteW;
	int spriteH = 31;
	float imageX = 320;
	float imageY = 128;
	float startDX;
	float startDY;
	int attack;

	Bullet(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, float xGoal, float yGoal)  
	{
		x = X;
		y = Y;
		goalX = xGoal;
		goalY = yGoal;
		speed = 0.8;
		life = true;
		w = W;
		h = H;
		name = Name;
		obj = lvl.GetObjects("solid");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
		imageX = 197;
		imageY = 5;
		speed = 0.3;
		sprite.setTextureRect(IntRect(imageX, imageY, w, h));
		attack = 10;
		startDX = xGoal - X;
		startDY = yGoal - Y;
		numFrames = 7;//даем скорость.этот объект всегда двигаетс€
	}


	void Update(float time)
	{
		float positionGoal = sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));//считаем дистанцию (длину от точки ј до точки Ѕ). формула длины вектора

		if (positionGoal > 2) //этим условием убираем дергание во врем€ конечной позиции спрайта
			{
			x += speed*time*(goalX - x)/ positionGoal;// / distance;//идем по иксу с помощью вектора нормали
			y += speed*time*(goalY - y)/ positionGoal; /// distance;//идем по игреку так же
			}
		else
		{
			goalX += startDX;
			goalY += startDY;
		}
		//x += dx*time;//само движение пули по х
		//y += dy*time;//по у

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) 
		{//проход по объектам solid
			if (GetRect().intersects(obj[i].rect)) //если этот объект столкнулс€ с пулей,
			{
				life = false;// то пул€ умирает
			}
		}
		currentFrame += time * 0.008;
		if (currentFrame >= numFrames)
		{
			currentFrame = 0;
		}
		sprite.setTextureRect(IntRect(imageX + w * int(currentFrame), imageY, w, h));
		sprite.setPosition(x + w / 2, y + h / 2);//задаетс€ позицию пуле
	}

	FloatRect GetRect()
	{//ф-ци€ получени€ пр€моугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ци€ нужна дл€ проверки столкновений 
	}
};