#include "Player.h"

void Player::Control(float time)
{
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		state = left;
		speed = 0.2f;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		state = right;
		speed = 0.2f;
	}
	if (((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) && (onGround))
	{
		state = jump;
		dy = -0.5f;
		onGround = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::E) && (onGround))
	{
		isTeleport = true;
	}
}

void Player::CheckCollisionWithMap(float Dx, float Dy)//ф ци€ проверки столкновений с картой
{
	for (int i = 0; i<obj.size(); i++)//проходимс€ по объектам
		if (GetRect().intersects(obj[i].rect))//провер€ем пересечение игрока с объектом
		{
			if (obj[i].name == "solid")//если встретили преп€тствие
			{
				if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0) { x = obj[i].rect.left - w - int(w*0.03); }
				if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
			}
			if (obj[i].name == "exit")
				isExit = true;
			else
				isExit = false;
		}
}

void Player::Update(float time, Vector2f pos, int portalH)
{
	Control(time);
	switch (state)
	{
	case right:
		dx = speed;
		sprite.setTextureRect(IntRect(imageX + w * (int)currentFrame, imageY + h * 2, w, h));
		break;
	case left:
		dx = -speed;
		sprite.setTextureRect(IntRect(imageX + w * (int)currentFrame, imageY + h, w, h));
		break;
	case jump:
		break;
	case stay:
	{
		sprite.setTextureRect(IntRect(imageX, imageY, w, h));
		break;
	}
	}
	currentFrame += time * speedChangedFrames;
	x += dx*time;
	CheckCollisionWithMap(dx, 0);
	y += dy*time;
	CheckCollisionWithMap(0, dy);
	sprite.setPosition(x + w / 2, y + h / 2);
	if (health <= 0)
	{
		if (heart > 0)
		{
			health = 5;
			heart--;
		}
		else
			life = false;
	}
	if (!isMove) speed = 0;
	dy += 0.0015f*time;
	teleportY = CooordinateYPortal(obj, pos, portalH);
	if (teleportY != -1)
	{
		openPortal = true;
		sprite.setColor(Color::Color(153, 50, 153, 255));
	}
	else
	{
		sprite.setColor(Color::Color(255, 255, 255, 255));
		openPortal = false;
	}
	if (isInvulnerability)
	{
		invulnerabilityTimer += time;
		if (currentFrame <= 1)
		{
			sprite.setColor(Color::Color(255, 255, 255, 100));
		}
		else
			sprite.setColor(Color::Color(255, 255, 255, 255));
		if (invulnerabilityTimer >= durationInvulnerability)
		{
			invulnerabilityTimer = 0;
			isInvulnerability = false;
		}
	}
	if (currentFrame >= 2 || !onGround)
	{
		currentFrame = 0;
	}

}

FloatRect Player::GetRect()
{//ф-ци€ получени€ пр€моугольника. его коорд,размеры (шир,высот).
	return FloatRect(x, y, float(w), float(h));//эта ф-ци€ нужна дл€ проверки столкновений 
}


int Player::CooordinateYPortal(std::vector<Object> obj, Vector2f pos, int portalH)//ф ци€ проверки столкновений с картой
{
	RectangleShape aim = CreateAim(pos);
	FloatRect rectAir = FloatRect(pos.x, pos.y + portalH, 1, 1);
	FloatRect rectEarth = FloatRect(pos.x, pos.y, 1, 1);
	for (int i = 0; i < obj.size(); i++)
	{//проходимс€ по объектам
		if (obj[i].name == "solid")
		{
			if (obj[i].rect.intersects(aim.getGlobalBounds()))
			{
				return -1;
			}
		}
	}
	for (int i = 0; i < obj.size(); i++)
	{//проходимс€ по объектам
		if (obj[i].name == "solid")
		{
			if (rectAir.intersects(obj[i].rect) && (!rectEarth.intersects(obj[i].rect)))
			{
				return int(obj[i].rect.top - portalH / 2);
			}
		}
	}
	return -1;
}

RectangleShape Player::CreateAim(Vector2f pos)
{
	float distance;
	distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//считаем дистанцию (длину от точки ј до точки Ѕ). формула длины вектора
	float dxLine = pos.x - x;//вектор , колинеарный пр€мой, котора€ пересекает спрайт и курсор
	float dyLine = pos.y - y;//он же, координата y
	float rotation = (atan2(dyLine, dxLine)) * float(180 / 3.14159265);
	RectangleShape line(sf::Vector2f(distance - float(0.05) * distance, float(1)));//получаем угол в радианах и переводим его в градусы
	line.rotate(rotation);
	line.setFillColor(Color::Green);
	if (state == right)
	{
		line.setSize(Vector2f(distance - w, 1));
		line.setPosition(x + w, y + h / 3);
	}
	if (state == left)
	{
		line.setPosition(x, y + float(h / 3));
	}
	if (state == stay || state == jump)
	{
		line.setPosition(x + float(w / 2), y + float(h / 3));
	}
	return line;
}

Vector2f Player::GetPos()
{
	return Vector2f(x, y);
}