#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "Player.h"
#include "Camera.h"

using namespace sf;

bool checkPosition(int pos1, int pos2)
{
	if (pos1 > pos2)
		return true;
	else
		return false;
}

bool openGorPortal(int x, int y, int up)
{
	bool flag = true;
	if (x - 75 > 0)
	{
		for (int i = (x - 75) / 32; i < (x + 75) / 32 / 32; i++) //от левого до правого края портала
			if (TileMap[int(y / 32) + up][i] != '0')
				return false;
	}
	else 
		return false;
	return true;
}

void openLPortal(Sprite portal, int x, int y, Color color)
{
	portal.setColor(color);
	portal.setPosition(x - 75, (int(y / 32)) * 32);
}

int main()
{
	bool x, y, L_portalis = false;
	RenderWindow window(VideoMode(640, 480), "Monsters, portals and things");
	camera.reset(FloatRect(0, 0, 640, 480));
	
	
	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Image portal_image;
	portal_image.loadFromFile("images/portal.png");
	portal_image.createMaskFromColor(Color(255, 255, 255));
	Texture portal;
	portal.loadFromImage(portal_image);
	Sprite L_portal;
	L_portal.setTexture(portal);
	L_portal.setTextureRect(IntRect(415, 247, 150, 32));
	Color White = Color(255, 255, 255);
	Color Blue = Color(153, 217, 234);

	Creature player("hero.png", 442, 225, 60.0, 69.0);
	float CurrentFrame = 0;

	Font font;
	font.loadFromFile("fonts/CyrilicOld.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);
	text.setStyle(Text::Bold | Text::Underlined);

	randomMapGenerate();
	Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		std::cout << time << "\n";
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//отрисовка персонажа
		if (player.life) {


			getplayercoordinateforview(player.getcreaturecoordinateX(), player.getcreaturecoordinateY());
			std::ostringstream healthString;    // объявили переменную
			healthString << player.health;		//занесли в нее число очков, то есть формируем строку
			text.setString("Здоровье: " + healthString.str());
			text.setPosition(camera.getCenter().x - 165, camera.getCenter().y - 200);
		}
		else
		{
			text.setString("GAME OVER");
			text.setPosition(camera.getCenter().x, camera.getCenter().y);
		}

		player.update(time);
		window.setView(camera);
		window.clear();
		//отрисовка карты
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));


				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		if (L_portalis)
			window.draw(L_portal);
		window.draw(player.sprite);
		window.draw(text);
		window.display();
	
	}

	return 0;
}