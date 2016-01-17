#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "Player.h"
#include "Camera.h"

using namespace sf;



int main()
{
	RenderWindow window(VideoMode(640, 480), "Monsters, portals and things");
	camera.reset(FloatRect(0, 0, 640, 480));
	
	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Creature player("hero.png", 442, 225, 60.0, 69.0);
	float CurrentFrame = 0;

	Font font;
	font.loadFromFile("fonts/CyrilicOld.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);
	text.setStyle(Text::Bold | Text::Underlined);

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
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player.dir = 1;
				player.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 10)
					CurrentFrame -= 10;
				player.sprite.setTextureRect(IntRect(67 * int(CurrentFrame), 521, 67, 70));

			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player.dir = 0;
				player.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 10)
					CurrentFrame -= 10;
				player.sprite.setTextureRect(IntRect(67 * int(CurrentFrame), 101, 67, 70));

			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				player.dir = 3;
				player.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 10)
					CurrentFrame -= 10;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				player.dir = 2;
				player.speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 10)
					CurrentFrame -= 10;
			}
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
		window.draw(player.sprite);
		window.draw(text);
		window.display();
	
	}

	return 0;
}