#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
using namespace sf;



class Creature 
{
public:
	std::vector<Object> obj;//������ �������� �����
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, onGround, isMove;
	Texture texture;
	Sprite sprite;
	String name;
	Creature(Image &image, String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	virtual void update(float time) = 0;
	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
};


class Player :public Creature {
public:
	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������
	int SpriteW = 33;
	int SpriteH = 30;
	float baseX = 290;
	float baseY = 128;
	Player(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Creature(image, Name, X, Y, W, H) 
	{
		playerScore = 0; state = stay;
		obj = lvl.GetAllObjects(); 
		if (name == "Player1") 
		{
				sprite.setTextureRect(IntRect(baseX + SpriteW, baseY, SpriteW, SpriteH));
		}
	}


		void control(float time)
		{
			float currentFrame = 0;
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				state = left;
				/*currentFrame += 0.005*time;
				if (currentFrame > 3) currentFrame -= 3;
				sprite.setTextureRect(IntRect(baseX + (SpriteW * int(currentFrame)), baseY, SpriteW, SpriteH));*/
				speed = 0.2;
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				state = right;
				speed = 0.2;
			}
			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround))
			{
				state = jump; dy = -0.7; onGround = false;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				state = down;
			}
		}

		void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
		{
			for (int i = 0; i<obj.size(); i++)//���������� �� ��������
				if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
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

		void update(float time)
		{
			control(time);
			switch (state)
			{
			case right: dx = speed; break;
			case left: dx = -speed; break;
			case up: break;
			case down: dx = 0; break;
			case jump: break;
			case stay: break;
			}
			x += dx*time;
			checkCollisionWithMap(dx, 0);
			y += dy*time;
			checkCollisionWithMap(0, dy);
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
			if (!isMove) speed = 0;
			if (life) { setPlayerCoordinateForView(x, y); }
			dy = dy + 0.0015*time;
		}
};

class Enemy :public Creature {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Creature(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(1, 81, w, h));
			dx = -0.1;//���� ��������.���� ������ ������ ���������
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}



	void update(float time)
	{
		if (name == "easyEnemy") {//��� ��������� � ����� ������ ������ ����� �����

								  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			x += dx*time;
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};