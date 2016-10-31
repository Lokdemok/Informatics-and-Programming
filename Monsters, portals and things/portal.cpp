#include "portal.h"

void Portal::Update(float time)
{
	currentFrame += time * speedChandegFrames;
	if (currentFrame >= 3)
	{
		currentFrame = 0;
		isOpen = true;
	}
	if (isOpen)
	{
		sprite.setTextureRect(IntRect(100 + size.x * (int)currentFrame, 150, size.x, size.y));
	}
	else
	{
		sprite.setTextureRect(IntRect(344 - size.x * (int)currentFrame, 150, size.x, size.y));
	}
	sprite.setPosition(position.x - size.x / 2, position.y - size.y / 2);
}

Vector2f Portal::GetCoordinates()
{
	return{ position };
}
Vector2i Portal::GetSize()
{
	return{ size };
}
FloatRect Portal::GetRect()
{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return FloatRect(position.x, position.y, float(size.x), float(size.y));//эта ф-ция нужна для проверки столкновений 
}