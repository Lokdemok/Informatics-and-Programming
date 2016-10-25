#include "Bullet.h"

void Bullet::Update(float time)
{
	float positionGoal = sqrt((goalX - x)*(goalX - x) + (goalY - y)*(goalY - y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������

	if (positionGoal > 10) //���� �������� ������� �������� �� ����� �������� ������� �������
	{
		x += speed*time*(goalX - x) / positionGoal;// / distance;//���� �� ���� � ������� ������� �������
		y += speed*time*(goalY - y) / positionGoal; /// distance;//���� �� ������ ��� ��
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

FloatRect Bullet::GetRect()
{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
	return FloatRect(x, y, float(w), float(h));//��� �-��� ����� ��� �������� ������������ 
}