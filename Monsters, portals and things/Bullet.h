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
		numFrames = 7;//���� ��������.���� ������ ������ ���������
	}


	void Update(float time)
	{
		float positionGoal = sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������

		if (positionGoal > 10) //���� �������� ������� �������� �� ����� �������� ������� �������
			{
			x += speed*time*(goalX - x)/ positionGoal;// / distance;//���� �� ���� � ������� ������� �������
			y += speed*time*(goalY - y)/ positionGoal; /// distance;//���� �� ������ ��� ��
			}
		else
		{
			goalX += startDX;
			goalY += startDY;
		}
		//x += dx*time;//���� �������� ���� �� �
		//y += dy*time;//�� �

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) 
		{//������ �� �������� solid
			if (GetRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}
		currentFrame += time * float(0.008);
		if (currentFrame >= numFrames)
		{
			currentFrame = 0;
		}
		sprite.setTextureRect(IntRect(imageX + w * int(currentFrame), imageY, w, h));
		sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
	}

	FloatRect GetRect()
	{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, float(w), float(h));//��� �-��� ����� ��� �������� ������������ 
	}
};