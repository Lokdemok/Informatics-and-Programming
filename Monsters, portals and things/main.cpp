#include <SFML/Graphics.hpp>
#include <iostream>
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


				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		window.draw(player.sprite);
		window.display();
	
	}

	return 0;
}