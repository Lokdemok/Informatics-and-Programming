#include "Enemy.h"

void Enemy::CheckCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
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



void Enemy::Update(float time, Vector2f pos)
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

void Enemy::SetScale(enemyStates newState, Vector2f direction)
{
	state = newState;
	sprite.setScale(direction);
}

bool Enemy::CheckWatchPlayer(std::vector<Object> obj, Vector2f pos)//� ��� �������� ������������ � ������
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

FloatRect Enemy::GetRect()
{
	return FloatRect(x, y, float(w), float(h));//��� �������� ������������ 
}