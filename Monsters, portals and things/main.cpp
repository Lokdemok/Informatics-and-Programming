#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "iostream"
#include "level.h"
#include "portal.h"
#include <vector>
#include <list>
#include "Player.h"
#include "Enemy.h"
//#include "map.h"


using namespace sf;
using namespace std;


void UpdateEnemies(vector<Enemy*> & enemy, float time, Player &hero)
{
	vector<Enemy*>::iterator it;
	for (it = enemy.begin(); it != enemy.end();)
	{
		Enemy *b = *it;
	//	b->Update(game.time, hero.GetPos().x, hero.GetPos().y);
		b->Update(time, Vector2f(hero.x, hero.y));
		if (!b->life)
		{
			it = enemy.erase(it);
			delete b;
		}
		else
			it++;
	}
}

void UpdatePortals(vector <Portal*> & portal, float time)
{
	vector<Portal*>::iterator it;
	for (it = portal.begin(); it != portal.end();)
	{
		Portal *b = *it;
		//	b->Update(game.time, hero.GetPos().x, hero.GetPos().y);
		b->Update(time);
		if (!b->life)
		{
			it = portal.erase(it);
			delete b;
		}
		else
			it++;
	}
}

void DrawEnemies(RenderWindow & window, vector<Enemy*> & enemy)
{
	vector<Enemy*>::iterator it_e;
	for (it_e = enemy.begin(); it_e != enemy.end(); it_e++)
		window.draw((*it_e)->sprite);
}

void DrawPortals(RenderWindow & window, vector<Portal*> & portal)
{
	vector<Portal*>::iterator it_p;
	for (it_p = portal.begin(); it_p != portal.end(); it_p++)
		window.draw((*it_p)->sprite);
}

void CreatePortal(vector<Portal*> & portals, Image portalImage, String name, Level lvl, Vector2f pos, int portalW, int portalH)
{
	vector<Portal*>::iterator it_p;
	for (it_p = portals.begin(); it_p != portals.end(); it_p++)
		if ((*it_p)->name == name)
		{
			(*it_p)->life = false;
		}
	portals.push_back(new Portal(portalImage, name, lvl, pos.x, pos.y, portalW, portalH));

}

void EntitiesIntersection(Player &hero, vector<Enemy*> &enemy, vector<Portal*> &portals)
{
	vector<Enemy*>::iterator it_e;
	vector<Portal*>::iterator it_p;
	for (it_e = enemy.begin(); it_e != enemy.end(); it_e++)
	{
		Enemy *enemy = *it_e;
		if (hero.GetRect().intersects(enemy->GetRect()))
		{
			//if (hero.dx < 0) { hero.x = enemy->x + enemy->w; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
			//if (hero.dx > 0) { hero.x = enemy->x - hero.w; }
			if (!hero.isInvulnerability)
			{
				hero.health -= enemy->attack;
				hero.isInvulnerability = true;
				std::cout << hero.health << "\n";
			}
			if (enemy->name == "EasyEnemy") 
			{//и при этом им€ объекта EasyEnemy,то..
				enemy->dx *= -1;
			}
		}
	}
}
void TeleportPlayer(Player &player, vector<Portal*> &portals)
{
	player.isTeleport = false;
	if (portals.size() >= 2)
	{
		String namePortal;
		vector<Portal*>::iterator it_p;
		for (it_p = portals.begin(); it_p != portals.end(); it_p++)
		{
			if (player.GetRect().intersects((*it_p)->GetRect()))
			{
				namePortal = (*it_p)->name;
			}
		}
		if (namePortal.getSize() != 0)
		{
			for (it_p = portals.begin(); it_p != portals.end(); it_p++)
			{
				if (!((*it_p)->name == namePortal))
				{
					player.x = (*it_p)->x - ((*it_p)->w / 2);
					player.y = (*it_p)->y - ((*it_p)->h / 2);
				}
			}
		}
	}
}


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

	Image flyEnemyImage;
	flyEnemyImage.loadFromFile("images/enemy2.png");
	flyEnemyImage.createMaskFromColor(Color(255, 255, 255));

	Image portalImage;
	portalImage.loadFromFile("images/portals.png");
	portalImage.createMaskFromColor(Color(0, 128, 0));
	int portalH = 49;
	int portalW = 35;

	Object playerObject = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player player(heroImage, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 32);//передаем координаты пр€моугольника player из карты в координаты нашего игрока

	vector <Enemy*>  enemies;//создаю список, сюда буду кидать объекты.например врагов.
	vector <Enemy*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе
	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		enemies.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 53, 29));//и закидываем в список всех наших врагов с карты

	e = lvl.GetObjects("flyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе
	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		enemies.push_back(new Enemy(flyEnemyImage, "flyEnemy", lvl, e[i].rect.left, e[i].rect.top, 38, 36));

	vector <Portal*> portals;
	vector <Portal*>::iterator it_p;
	Clock clock;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Event event;
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				pos.y = player.teleportY;
				if ((event.key.code == Mouse::Left) && (player.openPortal))
				{
					CreatePortal(portals, portalImage, "blue", lvl, pos, portalW, portalH);
				}
				else if (event.key.code == Mouse::Right && (player.openPortal))
				{
					CreatePortal(portals, portalImage, "yellow", lvl, pos, portalW, portalH);
				}
			}
			if (player.isTeleport)
			{
				TeleportPlayer(player, portals);
			}			
		}
		EntitiesIntersection(player, enemies, portals);
		if (player.life)
		{
			player.Update(time, pos, portalH);// Player update function	
			UpdateEnemies(enemies, time, player);
			UpdatePortals(portals, time);
		}
		window.setView(camera);
		window.clear();
		lvl.Draw(window);
		DrawEnemies(window, enemies);
		DrawPortals(window, portals);
		window.draw(player.sprite);
		window.display();
	}
	return 0;
}