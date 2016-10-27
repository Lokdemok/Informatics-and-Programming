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
	Texture allTexture;
	Text text;
};

struct Game
{
	bool isPause = true;
	int portalH = 50;
	int portalW = 36;
	Graphics graphic;
};