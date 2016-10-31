#ifndef player_h
#define player_h
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
//#include "Camera.h"
#include "level.h"
#include "game.h"
using namespace sf;


class Player 
{
private:
	Vector2f position;
	Vector2i size;
	Vector2i imagePos = Vector2i(0, 151);
	Vector2f translocation = Vector2f(0, 0);

public:
	float speed = 0;
	float currentFrame = 0;
	int health = 5;
	bool onGround = false;
	bool isMoving = false;
	bool isTeleporting = false;
	bool doesOpenPortal;
	bool isInvulnerability = false;
	float invulnerabilityTimer = 0;
	float countInvulnerability = 0;
	float durationInvulnerability = 3000;
	float teleportTimer = 0;
	float cooldownTeleport = 500;
	float speedChangedFrames = 0.005f;
	String name;
	Sprite sprite;
	bool alive = true;
	bool isExit = false;
	enum { left, right, jump, stay } state;//добавляем тип перечисления - состояние объекта
	int heart = 1;
	int teleportY;

	// TODO: move to C++
	Player(Texture &texture, String Name, float X, float Y, int W, int H)
		: state(stay)
		, position(X, Y)
		, size(W, H)
		, name(Name)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(float(size.x / 2), float(size.y / 2));
		if (name == "Player1")
		{
			sprite.setTextureRect(IntRect(imagePos.x + size.x, imagePos.y, size.x, size.y));
		}
	}


	void Control();
	void CheckCollisionWithMap(float Dx, float Dy, std::vector<Object>  & obj);
	void Update(std::vector<Object>  & obj, float time, Vector2f pos, int portalH);
	FloatRect GetRect();
	int CooordinateYPortal(std::vector<Object> obj, Vector2f pos, int portalH);//ф ция проверки столкновений с картой
	RectangleShape CreateAim(Vector2f pos);
	Vector2f GetPos();
	void SetPos(float x, float y);
};
#endif
