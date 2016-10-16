#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "iostream"
#include "level.h"
#include "portal.h"
#include <vector>
#include <list>
//#include "map.h"
#include "Camera.h"
#include "Player.h"

using namespace sf;

/*bool checkPosition(int pos1, int pos2)
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
		for (int i = (x - 75) / 32; i < (x + 75) / 32 / 32; i++) //от левого до правого кра€ портала
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
}*/

int main()
{
	RenderWindow window(VideoMode(640, 480), "Monsters, portals and things");
	camera.reset(FloatRect(0, 0, 640, 480));


	Level lvl;//создали экземпл€р класса уровень
	lvl.LoadFromFile("lev1.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.



	Image heroImage;
	heroImage.loadFromFile("images/hero1.png");
	heroImage.createMaskFromColor(Color(255, 255, 255));

    Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy1.png");
	easyEnemyImage.createMaskFromColor(Color(88, 152, 144));
	easyEnemyImage.createMaskFromColor(Color(255, 255, 255));

	Image portalImage;
	portalImage.loadFromFile("images/portal.png");
	portalImage.createMaskFromColor(Color(0, 128, 0));
	bool lp = false;

	Object playerObject = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player player(heroImage, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 31);//передаем координаты пр€моугольника player из карты в координаты нашего игрока

	std::list<Creature*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Creature*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе
	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 52, 26));//и закидываем в список всех наших врагов с карты

	Clock clock;
	while (window.isOpen())
	{

		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{
					Vector2i pixelPos = Mouse::getPosition(window);
					Vector2f pos = window.mapPixelToCoords(pixelPos);
					Portal leftPortal(portalImage, "Left", pos.x, pos.y, 39, 45);
					lp = true;
				}
			}
		}
		player.update(time);// Player update function	
		for (it = entities.begin(); it != entities.end(); it++) 
		{ 
			(*it)->update(time); 
		}
		if (lp)
			leftPortal.update(time);
		//easyEnemy.update(time);//easyEnemy update function
		window.setView(camera);
		window.clear();
		lvl.Draw(window);
		for (it = entities.begin(); it != entities.end(); it++) 
		{
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		window.draw(player.sprite);
		window.display();
	}
	return 0;
}