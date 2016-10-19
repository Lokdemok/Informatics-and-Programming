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
#include "Bullet.h"
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

void UpdateBullets(vector <Bullet*> & bullet, float time)
{
	vector<Bullet*>::iterator it;
	for (it = bullet.begin(); it != bullet.end();)
	{
		Bullet *b = *it;
		//	b->Update(game.time, hero.GetPos().x, hero.GetPos().y);
		b->Update(time);
		if (!b->life)
		{
			it = bullet.erase(it);
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

void DrawBullets(RenderWindow & window, vector<Bullet*> & bullet)
{
	vector<Bullet*>::iterator it_b;
	for (it_b = bullet.begin(); it_b != bullet.end(); it_b++)
		window.draw((*it_b)->sprite);
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

void EntitiesIntersection(Player &hero, vector<Enemy*> &enemy, vector<Portal*> &portals, vector<Bullet*> &bullets)
{
	vector<Enemy*>::iterator it_e;
	vector<Portal*>::iterator it_p;
	vector<Bullet*>::iterator it_b;
	for (it_e = enemy.begin(); it_e != enemy.end(); it_e++)
	{
		Enemy *enemy = *it_e;
		if (hero.GetRect().intersects(enemy->GetRect()))
		{
			//if (hero.dx < 0) { hero.x = enemy->x + enemy->w; }//���� ����������� � ������ � ����� ���� ����� �� ����������� ������
			//if (hero.dx > 0) { hero.x = enemy->x - hero.w; }
			if (!hero.isInvulnerability)
			{
				hero.health -= enemy->attack;
				hero.isInvulnerability = true;
				std::cout << hero.health << "\n";
			}
			if (enemy->name == "EasyEnemy") 
			{//� ��� ���� ��� ������� EasyEnemy,��..
				enemy->dx *= -1;
			}
		}
		for (it_b = bullets.begin(); it_b != bullets.end(); it_b++)
		{
			Bullet *bullet = *it_b;
			if (hero.GetRect().intersects(bullet->GetRect()) && (!hero.isInvulnerability))
			{
				hero.health -= bullet->attack;
				hero.isInvulnerability = true;
				std::cout << hero.health << "\n";
				bullet->life = false;
			}
			if (bullet->GetRect().intersects(enemy->GetRect()))
			{
				enemy->health -= bullet->attack;
				std::cout << "enemy attacked\n";
				bullet->life = false;
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


	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("lev1.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.



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

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet.png");
	bulletImage.createMaskFromColor(Color(163, 73, 164));

	Image portalImage;
	portalImage.loadFromFile("images/portals.png");
	portalImage.createMaskFromColor(Color(0, 128, 0));
	int portalH = 49;
	int portalW = 35;

	Object playerObject = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Player player(heroImage, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 32);//�������� ���������� �������������� player �� ����� � ���������� ������ ������

	vector <Enemy*>  enemies;//������ ������, ���� ���� ������ �������.�������� ������.
	vector <Enemy*>::iterator it_e;//�������� ����� ��������� �� ��-��� ������
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������
	for (int i = 0; i < e.size(); i++)//���������� �� ��������� ����� �������(� ������ �� ������)
		enemies.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 53, 29));//� ���������� � ������ ���� ����� ������ � �����

	e = lvl.GetObjects("flyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������
	for (int i = 0; i < e.size(); i++)//���������� �� ��������� ����� �������(� ������ �� ������)
		enemies.push_back(new Enemy(flyEnemyImage, "flyEnemy", lvl, e[i].rect.left, e[i].rect.top, 38, 36));

	vector <Portal*> portals;
	vector <Portal*>::iterator it_p;
	vector <Bullet*> bullets;
	vector <Bullet*>::iterator it_b;
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
				cout << "mouse\n";
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
				cout << "telepoet\n";
			}

		}
		for (it_e = enemies.begin(); it_e != enemies.end(); it_e++)
		{
			if ((*it_e)->name == "flyEnemy" && (*it_e)->isShoot)
			{
				float x, y;
				if ((*it_e)->x > player.x)
					x = (*it_e)->x;
				else
					x = (*it_e)->x + (*it_e)->w;
				if ((*it_e)->y > player.y)
					y = (*it_e)->y;
				else
					y = (*it_e)->y + (*it_e)->h;
				bullets.push_back(new Bullet(bulletImage, "bullet", lvl, x, y, 14, 14, player.x, player.y));
				cout << "shoot\n";
			}
		}
		EntitiesIntersection(player, enemies, portals, bullets);
		if (player.life)
		{
			player.Update(time, pos, portalH);// Player update function	
			UpdateEnemies(enemies, time, player);
			UpdatePortals(portals, time);
			UpdateBullets(bullets, time);
		}
		window.setView(camera);
		window.clear();
		lvl.Draw(window);
		DrawEnemies(window, enemies);
		DrawPortals(window, portals);
		DrawBullets(window, bullets);
		window.draw(player.sprite);
		window.display();
	}
	return 0;
}