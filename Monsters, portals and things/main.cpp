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
#include "game.h"
#include "Camera.h"



using namespace sf;
using namespace std;


string GetLevelNumb(Game &game)
{
	switch (game.level)
	{
	case 1: return "lev1.tmx"; break;
	default: game.isTrapLevel = true; return "level1.tmx"; break;
	}
}

void UpdateEnemies(vector<Enemy*> & enemy, float time, Player &hero)
{
	vector<Enemy*>::iterator it;
	for (it = enemy.begin(); it != enemy.end();)
	{
		Enemy *b = *it;
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
	// TODO: use Remove_if + erase
	vector<Bullet*>::iterator it;
	for (it = bullet.begin(); it != bullet.end();)
	{
		Bullet *b = *it;
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
	// TODO: use range-based for
	for (vector<Bullet*>::iterator it_b = bullet.begin(); it_b != bullet.end(); it_b++)
		window.draw((*it_b)->sprite);
}

void DrawStatistic(RenderWindow & window, Game *g, Player &player, View camera)
{
	int count = player.health;
	while (count != 0)
	{
		g->graphic.statistic.heart.setPosition(camera.getCenter().x - 320 + count * 20, camera.getCenter().y - 230);
		window.draw(g->graphic.statistic.heart);
		count--;
	}
	count = player.heart;
	while (count != 0)
	{
		Sprite life = g->graphic.statistic.life;
		life.setPosition(camera.getCenter().x - 80 + count * 18, camera.getCenter().y - 230);
		window.draw(life);
		count--;
	}
}

void CreatePortal(vector<Portal*> & portals, Game &game, String name, Level lvl, Vector2f pos, Texture &texture)
{
	// TODO: use range-based for
	vector<Portal*>::iterator it_p;
	for (it_p = portals.begin(); it_p != portals.end(); it_p++)
		if ((*it_p)->name == name)
		{
			(*it_p)->life = false;
		}
	portals.push_back(new Portal(texture, name, lvl, pos.x, pos.y, game.portalW, game.portalH));

}

void EntitiesIntersection(Player &hero, vector<Enemy*> &enemy, vector<Portal*> &portals, vector<Bullet*> &bullets, Sound &damage)
{
	vector<Enemy*>::iterator it_e;
	vector<Portal*>::iterator it_p;
	vector<Bullet*>::iterator it_b;
	for (it_e = enemy.begin(); it_e != enemy.end(); it_e++)
	{
		Enemy *enemy = *it_e;
		if (hero.GetRect().intersects(enemy->GetRect()))
		{
			if (!hero.isInvulnerability)
			{
				hero.health -= enemy->attack;
				hero.isInvulnerability = true;
				damage.play();
				if (enemy->name == "trap")
				{
					enemy->isAttack = true;
				}
			}
		}
		for (it_b = bullets.begin(); it_b != bullets.end(); it_b++)
		{
			Bullet *bullet = *it_b;
			if (hero.GetRect().intersects(bullet->GetRect()) && (!hero.isInvulnerability))
			{
				hero.health -= bullet->attack;
				hero.isInvulnerability = true;
				bullet->life = false;
				damage.play();
			}
			if (bullet->GetRect().intersects(enemy->GetRect()))
			{
				enemy->health -= bullet->attack;
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


void DrawMessage(RenderWindow &window, Text &text, const String str, float x, float y)
{
	text.setColor(Color::White);
	text.setString(str);
	text.setPosition(x, y);
	window.draw(text);
}

void DrawAllMessages(Player &player, Game &game, RenderWindow &window)
{
	if (!player.life)
	{
		DrawMessage(window, game.graphic.text, "GAME OVER", camera.getCenter().x - WINDOW_SIZE.x / 2 + 150, camera.getCenter().y - WINDOW_SIZE.y / 2 + 100);
		game.isPause = true;
	}
	if (game.isPause)
	{
		if (player.life)
		{
			DrawMessage(window, game.graphic.text, "ѕауза", camera.getCenter().x - 50, camera.getCenter().y - WINDOW_SIZE.y / 2 + 50);
			DrawMessage(window, game.graphic.text, "Enter - сн€ть с паузы", camera.getCenter().x - WINDOW_SIZE.x / 2 + 145, camera.getCenter().y - WINDOW_SIZE.y / 2 + 100);
		}
		DrawMessage(window, game.graphic.text, "Tab - перезапустить уровень", camera.getCenter().x - WINDOW_SIZE.x / 2 + 100, camera.getCenter().y - WINDOW_SIZE.y / 2 + 150);
		DrawMessage(window, game.graphic.text, "Esc - выйти", camera.getCenter().x - WINDOW_SIZE.x / 2 + 220, camera.getCenter().y - WINDOW_SIZE.y / 2 + 200);
	}
}

bool StartGame(RenderWindow & window, Game & game)
{
	Level lvl;
	lvl.LoadFromFile(GetLevelNumb(game));
	Image image;
	Texture texture;
	if (!image.loadFromFile("images/lvl1.png"))
		cout << "Error loading image from file " << endl;
	image.createMaskFromColor(Color(0, 128, 0));
	if (!texture.loadFromImage(image))
		cout << "Error loading texture from image " << endl;

	Sprite heartSprite;
	heartSprite.setTexture(texture);
	heartSprite.setTextureRect(IntRect(395, 151, 54, 46));
	heartSprite.setScale(0.3f, 0.3f);

	Sprite lifeSprite;
	lifeSprite.setTexture(texture);
	lifeSprite.setTextureRect(IntRect(457, 149, 29, 29));
	lifeSprite.setScale(0.8f, 0.8f);

	Font font;
	font.loadFromFile("fonts/pixel.ttf");
	Text text("", font, 25);

	game.graphic.statistic.heart = heartSprite;
	game.graphic.statistic.life = lifeSprite;
	game.graphic.text = text;
	game.isPause = true;
	game.restart = false;

	vector <Enemy*>  enemies;
	vector <Enemy*>::iterator it_e;
	std::vector<Object> e = lvl.GetObjects("easyEnemy");
	for (int i = 0; i < e.size(); i++)
		enemies.push_back(new Enemy(texture, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 53, 28));

	e = lvl.GetObjects("flyEnemy");
	for (int i = 0; i < e.size(); i++)
		enemies.push_back(new Enemy(texture, "flyEnemy", lvl, e[i].rect.left, e[i].rect.top, 38, 36));
	
	
	if (lvl.IsExist("trap"))
	{
		e = lvl.GetObjects("trap");
		for (int i = 0; i < e.size(); i++)
			enemies.push_back(new Enemy(texture, "trap", lvl, e[i].rect.left, e[i].rect.top, 32, 18));
		game.isTrapLevel = true;
	}


	vector <Portal*> portals;
	vector <Bullet*> bullets;
	Clock clock;

	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot(shootBuffer);

	SoundBuffer portalBuffer;
	portalBuffer.loadFromFile("sound/portal.wav");
	Sound portal(portalBuffer);

	SoundBuffer teleportBuffer;
	teleportBuffer.loadFromFile("sound/teleport.wav");
	Sound teleport(teleportBuffer);

	SoundBuffer damageBuffer;
	damageBuffer.loadFromFile("sound/damage.wav");
	Sound damage(damageBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/damage.wav");
	Sound gameOver(deathBuffer);

	Music music;
	music.openFromFile("sound/musicGame.ogg");
	music.play();

	Object playerObject = lvl.GetObject("player");
	Player player(texture, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 32);

	while (window.isOpen() && (!game.restart))
	{
		float time = float(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 800;
		Event event;
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (Keyboard::isKeyPressed(Keyboard::Escape) && game.isPause))
			{
				window.close();
				game.restart = false;
			}
			if (event.type == Event::MouseButtonPressed)
			{
				pos.y = float(player.teleportY);
				if ((event.key.code == Mouse::Left) && (player.openPortal))
				{
					CreatePortal(portals, game, "blue", lvl, pos, texture);
					portal.play();
				}
				else if (event.key.code == Mouse::Right && (player.openPortal))
				{
					CreatePortal(portals, game, "yellow", lvl, pos, texture);
					portal.play();
				}
			}
			if (player.isTeleport)
			{
				TeleportPlayer(player, portals);
				teleport.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				game.isPause = false;
			}
			if (Keyboard::isKeyPressed(Keyboard::P))
			{
				game.isPause = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Tab) && game.isPause) 
			{ 
				game.restart = true; 
			}
		}
		if (player.isExit)
		{
			game.level++;
			game.restart = true;
		}
		for (it_e = enemies.begin(); it_e != enemies.end(); it_e++)
		{
			if ((*it_e)->name == "flyEnemy" && (*it_e)->isShoot)
			{
				float x, y;
				if ((*it_e)->x > player.GetPos().x)
					x = (*it_e)->x;
				else
					x = (*it_e)->x + (*it_e)->w;
				if ((*it_e)->y > player.GetPos().y)
					y = (*it_e)->y;
				else
					y = (*it_e)->y + (*it_e)->h;
				bullets.push_back(new Bullet(texture, "bullet", lvl, x, y, 14, 14, player.GetPos().x, player.GetPos().y));
				shoot.play();
			}
		}
		EntitiesIntersection(player, enemies, portals, bullets, damage);
		if (player.life)
		{
			setPlayerCoordinateForView(player.GetPos().x, player.GetPos().y);
		}
		if (!game.isPause)
		{
			UpdateEnemies(enemies, time, player);
			UpdatePortals(portals, time);
			UpdateBullets(bullets, time);
			player.Update(time, pos, game.portalH);
		}
		window.setView(camera);
		window.clear();
		lvl.Draw(window);
		DrawEnemies(window, enemies);
		DrawPortals(window, portals);
		DrawBullets(window, bullets);
		DrawStatistic(window, &game, player, camera);
		window.draw(player.sprite);
		DrawAllMessages(player, game, window);
		music.setLoop(true);
		window.display();
	}
	return game.restart;
}

void RunningGame(RenderWindow & window, Game & game)
{//ф-ци€ перезагружает игру , если это необходимо
	if (StartGame(window, game)) 
	{ 
		RunningGame(window, game);
	}
}

int main()
{
	RenderWindow window(VideoMode((unsigned int)WINDOW_SIZE.x, (unsigned int)WINDOW_SIZE.y), "Monsters, portals and things");
	camera.reset(FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));
	Game game;
	RunningGame(window, game);
	return 0;
}