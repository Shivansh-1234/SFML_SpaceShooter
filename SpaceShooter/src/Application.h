#pragma once

#include "DEPENDENCIES.h"
#include "AssetManager.h"
#include "Entity.h"
#include "Vec2.h"

class Application
{
public:
	void run();

	Application();

	static Application* instance;
private:
	sf::RenderWindow        m_window;
	AssetManager            m_assetManager;
	std::shared_ptr<Entity> m_spaceShip;
	sf::Sprite              m_backgroundImage;
	sf::Text				m_uiText;

	std::vector<std::shared_ptr<Entity>>  m_enemyVec;

	//Note: This is not a good way to create bullets . The better way would have been to create a bullet class and an then create an object of it inside the entity class
	//      Would have allowed me to add more features (like moving bullets to playerPos) but I realized it too late and was lazy :P
	std::vector<std::shared_ptr<Entity>>  m_bulletVec;

	//Variables:-
	//handles delta time
	sf::Clock m_clock; 
	//the update function updates 144 times a second
	float MS_PER_UPDATE = 1.f / 144.f;

	//used as game timer
	sf::Clock m_gameTimerClock;
	//used to spawn enemies after given interval
	sf::Clock m_enemyClock; 
	//used to change color of spaceship from red back to normal after colliding with enemy
	sf::Clock m_spaceShipColorClock; 
	//used to spawn bullet from enemies after given interval
	sf::Clock m_enemyBulletIntervalClock;

	float m_spaceShipVelocity;
	float m_bulletVelocity;
	float m_enemyVelocity;
	float m_enemySpawnTime;
	float m_enemyBulletSpawnTime;
	int   m_spaceShipHealth;
	int   m_score;
	int   m_enemiesKilled;
	
	//end game variables
	bool  m_gameHasEnded;
	bool  m_getTime;
	float m_endTime;
	
	//methods
	void initWindow();
	void initVaribles();
	void initAssets();
	void loadAssets();

	Entity spawnBullet(sf::Vector2f pos, const std::string& name, sf::Color color = sf::Color::White);
	bool   checkCollision(std::shared_ptr<Entity>& e1, std::shared_ptr<Entity>& e2);

	void sMovement();
	void sCollision();
	void sShooting();
	void sEnemySpawner();
	void sBulletSpawner();
	void sUI();
	void sEndGame();

	void handleInput();
	void update();
	void render();
};