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
	int numFrames;
	float distanceView;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	bool life;
	bool watchPlayer;
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
			distanceView = 2000;
			watchPlayer = false;
			numFrames = 16;
		}
	}

	void CheckCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (GetRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0)
					{
						y = obj[i].rect.top - h;
						dy = 0;
					}
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w;  dx = -float(0.1); sprite.scale(-1, 1); }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = float(0.1); sprite.scale(-1, 1); }
				}
			}
	}



	void Update(float time, Vector2f pos)
	{
		currentFrame += time * speedChangeFrames;
		if (name == "easyEnemy")
		{
			x += dx*time;
			CheckCollisionWithMap(dx, 0);//������������ ������������ �� �
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
		}
		sprite.setTextureRect(IntRect(imageX + w * int(currentFrame), imageY, w, h));
		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
		if (health <= 0) { life = false; }
	}

	void SetScale(enemyStates newState, Vector2f direction)
	{
		state = newState;
		sprite.setScale(direction);
	}

	bool CheckWatchPlayer(std::vector<Object> obj, Vector2f pos)//� ��� �������� ������������ � ������
	{
		float distance;
		distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������
		if (distance <= distanceView)
		{
			float dxLine = pos.x - x;//������ , ����������� ������, ������� ���������� ������ � ������
			float dyLine = pos.y - y;//�� ��, ���������� y
			float rotation = (atan2(dyLine, dxLine)) * float(180 / 3.14159265);
			RectangleShape aim(sf::Vector2f(distance - float(0.05) * distance, 1));//�������� ���� � �������� � ��������� ��� � �������
			aim.rotate(rotation);
			aim.setPosition(x, y + h / 3);
			for (int i = 0; i < obj.size(); i++)
			{//���������� �� ��������
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
		return FloatRect(x, y, float(w), float(h));//��� �������� ������������ 
	}
};