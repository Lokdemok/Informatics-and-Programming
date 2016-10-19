#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
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
	float numFrames;
	float distanceView;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	Texture texture;
	bool life;
	bool watchPlayer;
	bool isShoot;
	bool isLeft;
	int spriteW;
	int spriteH = 31;
	float imageX = 320;
	float imageY = 128;
	float shootTimer;
	int countNextShoot;
	float timeRecharge = 3000;
	int attack;
	enum  enemyStates { left, right, stay } state;

	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) {
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		life = true;
		obj = lvl.GetAllObjects();
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
		if (name == "easyEnemy")
		{
			imageX = 0;
			imageY = 81;
			sprite.setTextureRect(IntRect(imageX, imageY, w, h));
			health = 100;
			attack = 10;
			dx = -0.1;
			numFrames = 9;
		}
		if (name == "flyEnemy")
		{
			imageX = 3;
			imageY = 1;
			sprite.setTextureRect(IntRect(imageX, imageY, w, h));
			health = 100;
			attack = 10;
			dx = 0;
			state = left;
			distanceView = 2000;
			watchPlayer = false;
			countNextShoot = 0;
			numFrames = 16;
		}
	}

	void CheckCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (GetRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0)
					{
						y = obj[i].rect.top - h;
						dy = 0;
					}
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}



	void Update(float time, Vector2f pos)
	{
		currentFrame += time * 0.008;
		if (name == "easyEnemy")
		{
			x += dx*time;
			CheckCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			if (currentFrame >= numFrames)
			{
				currentFrame = 0;
			}
		}

		if (name == "flyEnemy")
		{
			if (currentFrame >= numFrames)
			{
				currentFrame = 0;
				if (state == left && (!watchPlayer))
				{
					SetScale(right, Vector2f(-1, 1));
				}
				else if (state == right && (!watchPlayer))
				{
					SetScale(left, Vector2f(1, 1));
				}

			}
			if (shootTimer != 0)
			{
				isShoot = false;
				shootTimer += time;
				if (shootTimer >= timeRecharge)
				{
					shootTimer = 0;
				}
			}
			if (CheckWatchPlayer(obj, pos) )
			{
				watchPlayer = true;
				sprite.setColor(Color::Red);
				if (state == right && pos.x < x)
				{
					SetScale(left, Vector2f(1, 1));
				}
				if (state == left && pos.x > x)
				{
					SetScale(right, Vector2f(-1, 1));
				}
				if (shootTimer == 0)
				{
					isShoot = true;
					shootTimer = 1;
				}
			}
			else
			{
				watchPlayer = false;
				sprite.setColor(Color::White);
			}
			if (state == left)
				isLeft = true;
			else
				isLeft = false;
		}
		sprite.setTextureRect(IntRect(imageX + w * int(currentFrame), imageY, w, h));
		sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
		if (health <= 0) { life = false; }
	}

	void SetScale(enemyStates newState, Vector2f direction)
	{
		state = newState;
		sprite.setScale(direction);
	}

	bool CheckWatchPlayer(std::vector<Object> obj, Vector2f pos)//ф ция проверки столкновений с картой
	{
		float distance;
		distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
		if (distance <= distanceView)
		{
			float dxLine = pos.x - x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
			float dyLine = pos.y - y;//он же, координата y
			float rotation = (atan2(dyLine, dxLine)) * 180 / 3.14159265;
			RectangleShape aim(sf::Vector2f(distance - 0.05 * distance, 1));//получаем угол в радианах и переводим его в градусы
			aim.rotate(rotation);
			aim.setPosition(x, y + h / 3);
			for (int i = 0; i < obj.size(); i++)
			{//проходимся по объектам
				if (obj[i].name == "solid")
				{
					if (obj[i].rect.intersects(aim.getGlobalBounds()))
					{
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	FloatRect GetRect()
	{
		return FloatRect(x, y, w, h);//для проверки столкновений 
	}
};