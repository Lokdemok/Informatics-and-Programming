#pragma once
#define game_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "level.h"


using namespace sf;
using namespace std;

const Vector2f WINDOW_SIZE = { 640, 480 };


struct Statistic
{
	Sprite life;
	Sprite heart;
	Text heroLife;
};

struct Graphics
{
	Statistic statistic;
	Sprite easyEnemy;
	Sprite flyEnemy;
	Sprite player;
	Sprite bullet;
	Sprite portal;
	int portalH;
	int portalW;
};

struct Game
{
	Graphics gr;
};