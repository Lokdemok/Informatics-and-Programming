#include "Bullet.h"

void Bullet::Update(float time)
{
	float positionGoal = sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора

	if (positionGoal > 10) //этим условием убираем дергание во время конечной позиции спрайта
	{
		x += speed*time*(goalX - x) / positionGoal;// / distance;//идем по иксу с помощью вектора нормали
		y += speed*time*(goalY - y) / positionGoal; /// distance;//идем по игреку так же
	}
	else
	{
		goalX += startDX;
		goalY += startDY;
	}
	//x += dx*time;//само движение пули по х
	//y += dy*time;//по у

	if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
	if (y <= 0) y = 1;

	for (int i = 0; i < obj.size(); i++)
	{//проход по объектам solid
		if (GetRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
		{
			life = false;// то пуля умирает
		}
	}
	currentFrame += time * float(0.008);
	if (currentFrame >= numFrames)
	{
		currentFrame = 0;
	}
	sprite.setTextureRect(IntRect(imageX + w * int(currentFrame), imageY, w, h));
	sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
}

FloatRect Bullet::GetRect()
{//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return FloatRect(x, y, float(w), float(h));//эта ф-ция нужна для проверки столкновений 
}