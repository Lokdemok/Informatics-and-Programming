#include "Enemy.h"

void Enemy::CheckCollisionWithMap(float Dx, float Dy, std::vector <Object> & objects)//ф ция проверки столкновений с картой
{
	for (auto o : objects)
	{
		if (o.name == "solid" && GetRect().intersects(o.rect))
			{
				if (Dy>0)
				{
					position.y = o.rect.top - size.y;
					translocation.y = 0;
				}
				if (Dy<0) { position.y = o.rect.top + o.rect.height;   translocation.y = 0; }
				if (Dx>0) { position.x = o.rect.left - size.x;  translocation.x = -float(0.1); sprite.scale(-1, 1); }
				if (Dx<0) { position.x = o.rect.left + o.rect.width; translocation.x = float(0.1); sprite.scale(-1, 1); }
			}
	}
}



void Enemy::Update(float time, Vector2f pos, std::vector <Object> & obj)
{
	currentFrame += time * speedChangeFrames;
	if (name == "easyEnemy")
	{
		position.x += translocation.x * time;
		CheckCollisionWithMap(translocation.x, 0, obj);//обрабатываем столкновение по Х
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
			if (state == right && pos.x < position.x)
			{
				SetScale(left, Vector2f(1, 1));
			}
			if (state == left && pos.x > position.x)
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
	if (name == "trap")
	{
		if (isAttack)
		{
			currentFrame += time * speedChangeFrames;
		}
		else
			currentFrame = 0;
		if (currentFrame >= numFrames)
		{
			currentFrame = 0;
			isAttack = false;
		}
	}
	sprite.setTextureRect(IntRect(imagePos.x + size.x * int(currentFrame), imagePos.y, size.x, size.y));
	sprite.setPosition(position.x + size.x / 2, position.y + size.y / 2); //задаем позицию спрайта в место его центра
	if (health <= 0) { alive = false; }
}

void Enemy::SetScale(enemyStates newState, Vector2f direction)
{
	state = newState;
	sprite.setScale(direction);
}

bool Enemy::CheckWatchPlayer(std::vector<Object> & objects, Vector2f pos)//ф ция проверки столкновений с картой
{
	float distance;
	distance = std::hypot(pos.x - position.x, pos.y - position.y);//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
	if (distance <= distanceView)
	{
		Vector2f dLine(pos.x - position.x, pos.y - position.y);
		float rotation = (atan2(dLine.y, dLine.x)) * float(180 / 3.14159265);
		RectangleShape aim(sf::Vector2f(distance - float(0.05) * distance, 1));//получаем угол в радианах и переводим его в градусы
		aim.rotate(rotation);
		aim.setPosition(position.x, position.y + size.y / 3);
		for (auto obj : objects)
		{//проходимся по объектам
			if (obj.name == "solid")
			{
				if (obj.rect.intersects(aim.getGlobalBounds()))
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
	return FloatRect(position.x, position.y, float(size.x), float(size.y));//для проверки столкновений 
}