#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Camera.h"
using namespace sf;


class Player 
{
public:
	float dx; 
	float dy; 
	float speed; 
	float moveTimer;
	float currentFrame = 0;
	float x;
	float y;
	int health;
	bool onGround;
	bool isMove;
	bool isTeleport;
	bool openPortal;
	bool isInvulnerability;
	float countInvulnerability = 0;
	float durationInvulnerability = 300;
	int w;
	int h;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	Texture texture;
	bool life;
	enum { left, right, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������
	int SpriteW = 32;
	int SpriteH = 31;
	float baseX = 320;
	float baseY = 128;
	int teleportY;
	Player(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) 
	{
		playerScore = 0; 
		state = stay;
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		speed = 0; 
		health = 100; 
		dx = 0; 
		dy = 0;
		life = true; 
		onGround = false; 
		isMove = false;
		isTeleport = false;
		obj = lvl.GetAllObjects(); 
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
		if (name == "Player1") 
		{
			sprite.setTextureRect(IntRect(baseX, baseY, SpriteW, SpriteH));
		}
	}


		void Control(float time)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
			{
				state = left;
				speed = 0.2;
				std::cout << "left\n";
			}

			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
			{
				state = right;
				speed = 0.2;
				std::cout << "right\n";
			}
			if (((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) && (onGround))
			{
				state = jump; 
				dy = -0.7; 
				onGround = false;
				std::cout << "jump\n";
			}
			if (Keyboard::isKeyPressed(Keyboard::E) && (onGround))
			{
				isTeleport = true;
				std::cout << "open\n";
			}
		}

		void CheckCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
		{
			for (int i = 0; i<obj.size(); i++)//���������� �� ��������
				if (GetRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
				{
					if (obj[i].name == "solid")//���� ��������� �����������
					{
						if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
						if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
						if (Dx>0) { x = obj[i].rect.left - w - int(w*0.03); }
						if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
					}
				}
		}

		void Update(float time, Vector2f pos, int portalH)
		{
			Control(time);
			switch (state)
			{
				case right: 
					dx = speed; 
					sprite.setTextureRect(IntRect(baseX + w * (int)currentFrame, baseY + h * 2, w, h)); 
					break;
				case left: 
					dx = -speed; 
					sprite.setTextureRect(IntRect(baseX + w * (int)currentFrame, baseY + h, w, h));  
					break;
				case jump: 
					break;
				case stay: 
				{
					sprite.setTextureRect(IntRect(baseX, baseY, SpriteW, SpriteH));
					break;
				}
			}
			currentFrame += time * 0.005;
			x += dx*time;
			CheckCollisionWithMap(dx, 0);
			y += dy*time;
			CheckCollisionWithMap(0, dy);
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
			if (!isMove) speed = 0;
			if (life) { setPlayerCoordinateForView(x, y); }
			dy = dy + 0.0015*time;
			teleportY = CooordinateYPortal(obj, pos, portalH);
			if (teleportY != -1)
			{
				openPortal = true;
				sprite.setColor(Color::Color(153, 50, 153, 255));
			}
			else
			{
				sprite.setColor(Color::Color(255, 255, 255, 255));
				openPortal = false;
			}
			if (isInvulnerability)
			{
					if (currentFrame <= 1)
					{
						sprite.setColor(Color::Color(255, 255, 255, 100));
						countInvulnerability++;
					}
					else
						sprite.setColor(Color::Color(255, 255, 255, 255));
					if (countInvulnerability >= durationInvulnerability)
					{
						countInvulnerability = 0;
						isInvulnerability = false;
					}
			}
			if (currentFrame >= 2 || !onGround)
			{
				currentFrame = 0;
			}

		}

		FloatRect GetRect() 
		{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
			return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
		}


		int CooordinateYPortal(std::vector<Object> obj, Vector2f pos, int portalH)//� ��� �������� ������������ � ������
		{
			RectangleShape aim = CreateAim(pos);
			FloatRect rectAir = FloatRect(pos.x, pos.y + portalH, 1, 1);
			FloatRect rectEarth = FloatRect(pos.x, pos.y, 1, 1);
			for (int i = 0; i < obj.size(); i++)
			{//���������� �� ��������
				if (obj[i].name == "solid")
				{
					if (obj[i].rect.intersects(aim.getGlobalBounds()))
					{
						return -1;
					}
				}
			}
			for (int i = 0; i < obj.size(); i++)
			{//���������� �� ��������
				if (obj[i].name == "solid")
				{
					if (rectAir.intersects(obj[i].rect) && (!rectEarth.intersects(obj[i].rect)))
					{
						return obj[i].rect.top - portalH / 2;
					}
				}
			}
			return -1;
		}

		RectangleShape CreateAim(Vector2f pos)
		{
			float distance;
			distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������
			float dxLine = pos.x - x;//������ , ����������� ������, ������� ���������� ������ � ������
			float dyLine = pos.y - y;//�� ��, ���������� y
			float rotation = (atan2(dyLine, dxLine)) * 180 / 3.14159265;
			RectangleShape line(sf::Vector2f(distance - 0.05 * distance, 1));//�������� ���� � �������� � ��������� ��� � �������
			line.rotate(rotation);
			line.setFillColor(Color::Green);
			if (state == right)
			{
				line.setSize(Vector2f(distance - w, 1));
				line.setPosition(x + w, y + h / 3);
			}
			if (state == left)
			{
				line.setPosition(x, y + h / 3);
			}
			if (state == stay || state == jump)
			{
				line.setPosition(x + w / 2, y + h / 3);
			}
			return line;
		}
};

