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
public:
	float dx; 
	float dy; 
	float speed; 
	float currentFrame = 0;
	float x;
	float y;
	int health;
	bool onGround;
	bool isMove;
	bool isTeleport;
	bool openPortal;
	bool isInvulnerability;
	float invulnerabilityTimer = 0;
	float countInvulnerability = 0;
	float durationInvulnerability = 3000;
	float teleportTimer = 0;
	float cooldownTeleport = 500;
	float speedChangedFrames = 0.005f;
	int w;
	int h;
	String name;
	Sprite sprite;
	std::vector<Object> obj;
	bool life;
	bool isExit = false;
	enum { left, right, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������
	int imageX = 0;
	int imageY = 151;
	int heart;
	int teleportY;
	Player(Texture &texture, String Name, Level &lvl, float X, float Y, int W, int H)
	{
		playerScore = 0; 
		state = stay;
		x = X;
		y = Y;
		w = W;
		h = H;
		name = Name;
		speed = 0; 
		health = 5; 
		heart = 1;
		dx = 0; 
		dy = 0;
		life = true; 
		onGround = false; 
		isMove = false;
		isTeleport = false;
		obj = lvl.GetAllObjects(); 
		sprite.setTexture(texture);
		sprite.setOrigin(float(w / 2), float(h / 2));
		if (name == "Player1") 
		{
			sprite.setTextureRect(IntRect(imageX + w, imageY, w, h));
		}
	}


	void Control();
	void CheckCollisionWithMap(float Dx, float Dy);
	void Update(float time, Vector2f pos, int portalH);
	FloatRect GetRect();
	int CooordinateYPortal(std::vector<Object> obj, Vector2f pos, int portalH);//� ��� �������� ������������ � ������
	RectangleShape CreateAim(Vector2f pos);
	Vector2f GetPos();
};
#endif
