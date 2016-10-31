#include "Bullet.h"

void Bullet::Update(float time, std::vector <Object> & objects)
{
	float positionGoal = std::hypot(goal.x - position.x, goal.y - position.y);//считаем дистанцию (длину от точки А до точки Б). формула длины вектора

	if (positionGoal > 10) //этим условием убираем дергание во время конечной позиции спрайта
	{
		position += speed*time*(goal - position) / positionGoal;
	}
	else
	{
		goal += translocation;
	}

	if (position.x <= 0) position.x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
	if (position.y <= 0) position.y = 1;

	for (auto o : objects)
	{
		if (o.name == "solid" && GetRect().intersects(o.rect))
		{
			alive = false;// то пуля умирает
		}
	}
	currentFrame += time * float(0.008);
	if (currentFrame >= numFrames)
	{
		currentFrame = 0;
	}
	sprite.setTextureRect(IntRect(imagePosition.x + size.x * int(currentFrame), imagePosition.y, size.x, size.y));
	sprite.setPosition(position.x + size.x / 2, position.y + size.y / 2);//задается позицию пуле
}

FloatRect Bullet::GetRect()
{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return FloatRect(position.x, position.y, float(size.x), float(size.y));//эта ф-ция нужна для проверки столкновений 
}