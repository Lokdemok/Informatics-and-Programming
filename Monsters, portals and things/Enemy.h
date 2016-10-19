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
	int spriteW;
	int spriteH = 31;
	float imageX = 320;
	float imageY = 128;
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
			numFrames = 9;//���� ��������.���� ������ ������ ���������
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
			distanceView = 300;
			watchPlayer = false;
			numFrames = 16;//���� ��������.���� ������ ������ ���������
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
					if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}



	void Update(float time, Vector2f pos)
	{
		if (name == "easyEnemy")
		{//��� ��������� � ����� ������ ������ ����� �����

		 //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			x += dx*time;
			CheckCollisionWithMap(dx, 0);//������������ ������������ �� �
			currentFrame += time * 0.008;
			if (currentFrame >= numFrames)
			{
				currentFrame = 0;
			}
		}

		if (name == "flyEnemy")
		{
			currentFrame += time * 0.008;
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
			if (CheckWatchPlayer(obj, pos))
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
		//��� ��������� � ����� ������ ������ ����� �����

		//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
		//x += dx*time;
		//CheckCollisionWithMap(dx, 0);//������������ ������������ �� �
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
			float rotation = (atan2(dyLine, dxLine)) * 180 / 3.14159265;
			RectangleShape aim(sf::Vector2f(distance - 0.05 * distance, 1));//�������� ���� � �������� � ��������� ��� � �������
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
		}
		return true;
	}

	RectangleShape CreateAim(Vector2f pos)
	{
		float distance;
		distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������
		watchPlayer == true;
		float dxLine = pos.x - x;//������ , ����������� ������, ������� ���������� ������ � ������
		float dyLine = pos.y - y;//�� ��, ���������� y
		float rotation = (atan2(dyLine, dxLine)) * 180 / 3.14159265;
		RectangleShape line(sf::Vector2f(distance - 0.05 * distance, 1));//�������� ���� � �������� � ��������� ��� � �������
		line.rotate(rotation);
		line.setPosition(x, y + h / 3);
		return line;
	}

	FloatRect GetRect()
	{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
};