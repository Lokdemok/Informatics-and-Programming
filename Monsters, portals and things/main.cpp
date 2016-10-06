#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "level.h"
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
		for (int i = (x - 75) / 32; i < (x + 75) / 32 / 32; i++) //�� ������ �� ������� ���� �������
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


	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("lev1.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.



	Image heroImage;
	heroImage.loadFromFile("images/hero1.png");
	heroImage.createMaskFromColor(Color(255, 255, 255));

    Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy1.png");
	easyEnemyImage.createMaskFromColor(Color(88, 152, 144));
	easyEnemyImage.createMaskFromColor(Color(255, 255, 255));

	Object playerObject = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Object easyEnemyObject = lvl.GetObject("easyEnemy");
	Player player(heroImage, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 31);//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 52, 26);//�������� ���������� �������������� easyEnemy �� ����� � ���������� ������ �����

	/*std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
	std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������
	*/

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
		}
		player.update(time);// Player update function	
		easyEnemy.update(time);//easyEnemy update function
		window.setView(camera);
		window.clear();
		lvl.Draw(window);
		window.draw(easyEnemy.sprite);
		window.draw(player.sprite);
		window.display();
	}
	return 0;
}