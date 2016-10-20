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
//#include "map.h"


using namespace sf;
using namespace std;


struct GraphicResource {
	Image* image;
	Texture* texture;
	Sprite* sprite;
};

struct Heart
{
	Sprite sprite;
	int imageX = 3;
	int imageY = 10;
	int w = 54;
	int h = 46;
};

Sprite GetHeartSprite()
{
	Heart heart;
	return heart.sprite;
}

void CleanMemory(GraphicResource & gr)
{
	delete gr.image;
	delete gr.texture;
	delete gr.sprite;
}

GraphicResource MakeSprite(String const& fileName)
{
	GraphicResource gr;
	gr.image = new Image();
	gr.texture = new Texture();
	gr.sprite = new Sprite();
	if (!gr.image->loadFromFile(fileName))
		cout << "Error loading image from file " << endl;
	if (!gr.texture->loadFromImage(*gr.image))
		cout << "Error loading texture from image " << endl;
	gr.sprite->setTexture(*gr.texture);
	gr.image-> ~Image();
	return gr;
}


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

void DrawStatistic(RenderWindow & window, Game *g, Player &player, View camera)
{
	int count = player.health;
	while (count != 0)
	{
		g->gr.statistic.heart.setPosition(camera.getCenter().x - 320 + count * 20, camera.getCenter().y - 230);
		window.draw(g->gr.statistic.heart);
		count--;
	}
	count = player.heart;
	while (count != 0)
	{
		Sprite life = g->gr.statistic.life;
		life.setPosition(camera.getCenter().x - 80 + count * 18, camera.getCenter().y - 230);
		window.draw(life);
		count--;
	}
}

void CreatePortal(vector<Portal*> & portals, Game &game, String name, Level lvl, Vector2f pos)
{
	vector<Portal*>::iterator it_p;
	for (it_p = portals.begin(); it_p != portals.end(); it_p++)
		if ((*it_p)->name == name)
		{
			(*it_p)->life = false;
		}
	portals.push_back(new Portal(game.gr.portal, name, lvl, pos.x, pos.y, game.gr.portalW, game.gr.portalH));

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
			//if (hero.dx < 0) { hero.x = enemy->x + enemy->w; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
			//if (hero.dx > 0) { hero.x = enemy->x - hero.w; }
			if (!hero.isInvulnerability)
			{
				hero.health -= enemy->attack;
				hero.isInvulnerability = true;
				damage.play();
			}
			if (enemy->name == "EasyEnemy") 
			{//и при этом им€ объекта EasyEnemy,то..
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



Text MakeMessage(string string, int size)
{
	Font font;
	font.loadFromFile("fonts/pixel.ttf");
	std::ostringstream message;   // объ€вили переменную здоровь€ и времени
	message << string;
	Text text(message.str(), font, size);
	return text;
}

void GameOver(RenderWindow & window, View &view, Text &text)
{
	//Text text = MakeMessage("GAME OVER", 20);
	text.setColor(Color::White);
	text.setStyle(Text::Bold);
	text.setString("GAME OVER");//?????? ?????? ??????
	text.setPosition(view.getCenter().x - 65, view.getCenter().y - 20);//задаем позицию текста, отступа€ от центра камеры
	window.draw(text);//рисую этот текст
}

void RunningGame(RenderWindow & window)
{
	Level lvl;//создали экземпл€р класса уровень
	lvl.LoadFromFile("lev1.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.



	Image heroImage;
	heroImage.loadFromFile("images/hero1.png");
	heroImage.createMaskFromColor(Color(255, 255, 255));
	Texture heroTexture;
	heroTexture.loadFromImage(heroImage);
	Sprite heroSprite;
	heroSprite.setTexture(heroTexture);

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy1.png");
	easyEnemyImage.createMaskFromColor(Color(88, 152, 144));
	easyEnemyImage.createMaskFromColor(Color(255, 255, 255));
	Texture easyEnemyTex;
	easyEnemyTex.loadFromImage(easyEnemyImage);
	Sprite easyEnemySprite;
	easyEnemySprite.setTexture(easyEnemyTex);

	Image flyEnemyImage;
	flyEnemyImage.loadFromFile("images/enemy2.png");
	flyEnemyImage.createMaskFromColor(Color(255, 255, 255));
	Texture flyEnemyTex;
	flyEnemyTex.loadFromImage(flyEnemyImage);
	Sprite flyEnemySprite;
	flyEnemySprite.setTexture(flyEnemyTex);

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet.png");
	bulletImage.createMaskFromColor(Color(163, 73, 164));
	Texture bulletTexture;
	bulletTexture.loadFromImage(bulletImage);
	Sprite bulletSprite;
	bulletSprite.setTexture(bulletTexture);

	Image portalImage;
	portalImage.loadFromFile("images/portals.png");
	portalImage.createMaskFromColor(Color(0, 128, 0));
	Texture portalTexture;
	portalTexture.loadFromImage(portalImage);
	Sprite portalSprite;
	portalSprite.setTexture(portalTexture);


	Image heartImage;
	heartImage.loadFromFile("images/heart.png");
	heartImage.createMaskFromColor(Color(163, 73, 164));
	Texture heartTexture;
	heartTexture.loadFromImage(heartImage);
	Sprite heartSprite;
	heartSprite.setTexture(heartTexture);
	heartSprite.setTextureRect(IntRect(3, 10, 54, 46));  //приведение типов, размеры картинки исходные
	heartSprite.setScale(0.3f, 0.3f);//чуть уменьшили картинку, => размер стал меньше


	Image lifeImage;
	lifeImage.loadFromFile("images/life.png");
	lifeImage.createMaskFromColor(Color(163, 73, 164));
	Texture lifeTexture;
	lifeTexture.loadFromImage(lifeImage);
	Sprite lifeSprite;
	lifeSprite.setTexture(lifeTexture);
	lifeSprite.setTextureRect(IntRect(3, 2, 29, 29));  //приведение типов, размеры картинки исходные
	lifeSprite.setScale(0.8f, 0.8f);

	Font font;
	font.loadFromFile("fonts/pixel.ttf");
	Text text("", font, 25);

	Game game;
	game.gr.statistic.heart = heartSprite;
	game.gr.statistic.life = lifeSprite;
	game.gr.player = heroSprite;
	game.gr.easyEnemy = easyEnemySprite;
	game.gr.flyEnemy = flyEnemySprite;
	game.gr.bullet = bulletSprite;
	game.gr.portal = portalSprite;
	game.gr.portalH = 50;
	game.gr.portalW = 36;



	Object playerObject = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player player(game.gr.player, "Player1", lvl, playerObject.rect.left, playerObject.rect.top, 32, 32);//передаем координаты пр€моугольника player из карты в координаты нашего игрока

	vector <Enemy*>  enemies;
	vector <Enemy*>::iterator it_e;//итератор чтобы проходить по эл-там списка
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе
	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		enemies.push_back(new Enemy(game.gr.easyEnemy, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 53, 29));//и закидываем в список всех наших врагов с карты

	e = lvl.GetObjects("flyEnemy");//все объекты врага на tmx карте хран€тс€ в этом векторе
	for (int i = 0; i < e.size(); i++)//проходимс€ по элементам этого вектора(а именно по врагам)
		enemies.push_back(new Enemy(game.gr.flyEnemy, "flyEnemy", lvl, e[i].rect.left, e[i].rect.top, 38, 36));

	vector <Portal*> portals;
	vector <Portal*>::iterator it_p;
	vector <Bullet*> bullets;
	vector <Bullet*>::iterator it_b;
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

	Music music;//создаем объект музыки
	music.openFromFile("sound/musicGame.ogg");//загружаем файл
	music.play();

	while (window.isOpen())
	{
		float time = float(clock.getElapsedTime().asMicroseconds());
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
				pos.y = float(player.teleportY);
				if ((event.key.code == Mouse::Left) && (player.openPortal))
				{
					CreatePortal(portals, game, "blue", lvl, pos);
					portal.play();
				}
				else if (event.key.code == Mouse::Right && (player.openPortal))
				{
					CreatePortal(portals, game, "yellow", lvl, pos);
					portal.play();
				}
			}
			if (player.isTeleport)
			{
				TeleportPlayer(player, portals);
				teleport.play();
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
				bullets.push_back(new Bullet(game.gr.bullet, "bullet", lvl, x, y, 14, 14, player.x, player.y));
				shoot.play();
			}
		}
		EntitiesIntersection(player, enemies, portals, bullets, damage);
		if (player.life)
		{
			player.Update(time, pos, game.gr.portalH);// Player update function	
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
		DrawStatistic(window, &game, player, camera);
		window.draw(player.sprite);
		music.setLoop(true);
		if (!player.life)
		{
			GameOver(window, camera, text);
		}
		window.display();
	}
}


int main()
{
	RenderWindow window(VideoMode(640, 480), "Monsters, portals and things");
	camera.reset(FloatRect(0, 0, 640, 480));
	RunningGame(window);
	return 0;
}