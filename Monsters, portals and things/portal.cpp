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
		sprite.setTextureRect(IntRect(100 + w * (int)currentFrame, 150, w, h));
	}
	else
	{
		sprite.setTextureRect(IntRect(344 - w * (int)currentFrame, 150, w, h));
	}
	sprite.setPosition(x - w / 2, y - h / 2);
}

Vector2f Portal::GetCoordinates(float X, float Y)
{
	return{ x, y };
}
FloatRect Portal::GetRect()
{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return FloatRect(x, y, float(w), float(h));//эта ф-ция нужна для проверки столкновений 
}