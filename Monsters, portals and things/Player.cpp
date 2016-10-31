#include "Player.h"

void Player::Control()
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
		translocation.y = -0.5f;
		onGround = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::E) && (onGround) && teleportTimer == 0)
	{
		isTeleporting = true;
	}
}

void Player::CheckCollisionWithMap(float Dx, float Dy, std::vector<Object>  & obj)//ф ци€ проверки столкновений с картой
{
	for (auto o : obj)//проходимс€ по объектам
		if (GetRect().intersects(o.rect))//провер€ем пересечение игрока с объектом
		{
			if (o.name == "solid")//если встретили преп€тствие
			{
				if (Dy>0) { position.y = o.rect.top - size.y;  translocation.y = 0; onGround = true; }
				if (Dy<0) { position.y = o.rect.top + o.rect.height;   translocation.y = 0; }
				if (Dx>0) { position.x = o.rect.left - size.x - int(size.x * 0.03); }
				if (Dx<0) { position.x = o.rect.left + o.rect.width; }
			}
			if (o.name == "exit")
				isExit = true;
			else
				isExit = false;
		}
}

void Player::Update(std::vector<Object>  & obj, float time, Vector2f pos, int portalH)
{
	Control();
	switch (state)
	{
	case right:
		translocation.x = speed;
		sprite.setTextureRect(IntRect(imagePos.x + size.x * (int)currentFrame, imagePos.y + size.y * 2, size.x, size.y));
		break;
	case left:
		translocation.x = -speed;
		sprite.setTextureRect(IntRect(imagePos.x + size.x * (int)currentFrame, imagePos.y + size.y, size.x, size.y));
		break;
	case jump:
		break;
	case stay:
	{
		sprite.setTextureRect(IntRect(imagePos.x, imagePos.y, size.x, size.y));
		break;
	}
	}
	currentFrame += time * speedChangedFrames;
	position.x += translocation.x * time;
	CheckCollisionWithMap(translocation.x, 0, obj);
	position.y += translocation.y * time;
	CheckCollisionWithMap(0, translocation.y, obj);
	sprite.setPosition(position.x + size.x / 2, position.y + size.y / 2);
	if (health <= 0)
	{
		if (heart > 0)
		{
			health = 5;
			heart--;
		}
		else
			alive = false;
	}
	if (teleportTimer != 0)
	{
		teleportTimer += time;
		if (teleportTimer >= cooldownTeleport)
			teleportTimer = 0;
	}
	if (!isMoving) speed = 0;
	translocation.y += 0.0015f*time;
	teleportY = CooordinateYPortal(obj, pos, portalH);
	if (teleportY != -1)
	{
		doesOpenPortal = true;
		sprite.setColor(Color::Color(153, 50, 153, 255));
	}
	else
	{
		sprite.setColor(Color::Color(255, 255, 255, 255));
		doesOpenPortal = false;
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
	return FloatRect(position.x, position.y, float(size.x), float(size.y));//эта ф-ци€ нужна дл€ проверки столкновений 
}


int Player::CooordinateYPortal(std::vector<Object> obj, Vector2f pos, int portalH)//ф ци€ проверки столкновений с картой
{
	RectangleShape aim = CreateAim(pos);
	FloatRect rectAir = FloatRect(pos.x, pos.y + portalH, 1, 1);
	FloatRect rectEarth = FloatRect(pos.x, pos.y, 1, 1);
	for (auto o : obj)
	{//проходимс€ по объектам
		if (o.name == "solid")
		{
			if (o.rect.intersects(aim.getGlobalBounds()))
			{
				return -1;
			}
		}
	}
	for (auto o : obj)
	{//проходимс€ по объектам
		if (o.name == "solid")
		{
			if (rectAir.intersects(o.rect) && (!rectEarth.intersects(o.rect)))
			{
				return int(o.rect.top - portalH / 2);
			}
		}
	}
	return -1;
}

RectangleShape Player::CreateAim(Vector2f pos)
{
	float distance;
	distance = std::hypot((pos.x - position.x), (pos.y - position.y));//считаем дистанцию (длину от точки ј до точки Ѕ). формула длины вектора
	Vector2f dLine(pos.x - position.x, pos.y - position.y); //вектор , колинеарный пр€мой, котора€ пересекает спрайт и курсор
	float rotation = (atan2(dLine.y, dLine.x)) * float(180 / 3.14159265);
	RectangleShape line(sf::Vector2f(distance - float(0.05) * distance, float(1)));//получаем угол в радианах и переводим его в градусы
	line.rotate(rotation);
	line.setFillColor(Color::Green);
	if (state == right)
	{
		line.setSize(Vector2f(distance - size.x, 1));
		line.setPosition(position.x + size.x, position.y + size.y / 3);
	}
	if (state == left)
	{
		line.setPosition(position.x, position.y + float(size.y / 3));
	}
	if (state == stay || state == jump)
	{
		line.setPosition(position.x + float(size.x / 2), position.y + float(size.y / 3));
	}
	return line;
}

Vector2f Player::GetPos()
{
	return position;
}

void Player::SetPos(float x, float y)
{
	position.x = x;
	position.y = y;
}