#include "Bullet.h"

void Bullet::Update(float time, std::vector <Object> & objects)
{
	float positionGoal = std::hypot(goal.x - position.x, goal.y - position.y);//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������

	if (positionGoal > 10) //���� �������� ������� �������� �� ����� �������� ������� �������
	{
		position += speed*time*(goal - position) / positionGoal;
	}
	else
	{
		goal += translocation;
	}

	if (position.x <= 0) position.x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
	if (position.y <= 0) position.y = 1;

	for (auto o : objects)
	{
		if (o.name == "solid" && GetRect().intersects(o.rect))
		{
			alive = false;// �� ���� �������
		}
	}
	currentFrame += time * float(0.008);
	if (currentFrame >= numFrames)
	{
		currentFrame = 0;
	}
	sprite.setTextureRect(IntRect(imagePosition.x + size.x * int(currentFrame), imagePosition.y, size.x, size.y));
	sprite.setPosition(position.x + size.x / 2, position.y + size.y / 2);//�������� ������� ����
}

FloatRect Bullet::GetRect()
{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
	return FloatRect(position.x, position.y, float(size.x), float(size.y));//��� �-��� ����� ��� �������� ������������ 
}