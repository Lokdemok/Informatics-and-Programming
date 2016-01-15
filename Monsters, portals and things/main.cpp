#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

using namespace sf;



int main()
{
	Image image;
	image.loadFromFile("images/hero.png");
	image.createMaskFromColor(Color(136, 56, 168));

	Texture legtexture;
	legtexture.loadFromImage(image);

	Sprite legs;
	legs.setTexture(legtexture);
	legs.setTextureRect(IntRect(0, 180, 60, 40));
	legs.setPosition(250, 277);
	float CurrentFrame = 0;

	RenderWindow window(VideoMode(640, 480), "Monsters, portals and things");
	Creature player("hero.png", 250, 250, 96.0, 96.0);

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

		if (Keyboard::isKeyPressed(Keyboard::Left)) 
		{ 
			player.dir = 1; 
			player.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 10) 
				CurrentFrame -= 10;
			legs.setScale(-1, 1);
			legs.setTextureRect(IntRect(67 * int(CurrentFrame), 180, 67, 40));
			legs.move(-0.1*time, 0);
			player.head.setScale(-1, 1);
			player.head.setTextureRect(IntRect(67 * int(CurrentFrame), 100, 67, 46));

		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) 
		{
			player.dir = 0;
			player.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 10)
				CurrentFrame -= 10;
			legs.setScale(1, 1);
			legs.setTextureRect(IntRect(67 * int(CurrentFrame), 180, 67, 40));
			legs.move(0.1*time, 0);
			player.head.setScale(1, 1);
			player.head.setTextureRect(IntRect(67 * int(CurrentFrame), 100, 67, 46));

		} 
		if (Keyboard::isKeyPressed(Keyboard::Up)) 
		{
			player.dir = 3;
			player.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 10)
				CurrentFrame -= 10;
			legs.setTextureRect(IntRect(67 * int(CurrentFrame), 180, 67, 40));
			legs.move(0, -0.1*time);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) 
		{
			player.dir = 2;
			player.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 10)
				CurrentFrame -= 10;
			legs.setTextureRect(IntRect(67 * int(CurrentFrame), 180, 67, 40));
			legs.move(0, 0.1*time);
		}

		player.update(time);
	
		window.clear();
		window.draw(legs); 
		window.draw(player.head);
		window.display();
	
	}

	return 0;
}