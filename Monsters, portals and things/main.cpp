#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Monsters, portals and things");

	Image heroimage;
	heroimage.loadFromFile("images/hero.png");

	Texture herotexture;
	herotexture.loadFromImage(heroimage);

	Sprite herosprite;
	herosprite.setTexture(herotexture);
	herosprite.setTextureRect(IntRect(0, 192, 96, 96));
	herosprite.setPosition(250, 250);

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

		if (Keyboard::isKeyPressed(Keyboard::Left)) { herosprite.move(-0.1*time, 0); herosprite.setTextureRect(IntRect(0, 96, 96, 96)); }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { herosprite.move(0.1*time, 0); herosprite.setTextureRect(IntRect(0, 192, 96, 96)); }
		if (Keyboard::isKeyPressed(Keyboard::Up)) { herosprite.move(0, -0.1*time); herosprite.setTextureRect(IntRect(0, 288, 96, 96)); }
		if (Keyboard::isKeyPressed(Keyboard::Down)) { herosprite.move(0, 0.1*time); herosprite.setTextureRect(IntRect(0, 0, 96, 96)); }
	
		window.clear();
		window.draw(herosprite);
		window.display();
	
	}

	return 0;
}