#include "Application.h"

Application* Application::instance = nullptr;

Application::Application()
{
	initWindow();
	loadAssets();
	initAssets();
	initVaribles();
}

void Application::run()
{
	float lastTime = m_clock.getElapsedTime().asSeconds();
	float lag = 0.f;

	while (m_window.isOpen())
	{
		handleInput();

		//--------------------------------------------------
		//https://gameprogrammingpatterns.com/game-loop.html
		//--------------------------------------------------

		float current = m_clock.getElapsedTime().asSeconds();
		float elapsed = current - lastTime;
		lastTime = current;
		lag += elapsed;

		while (lag >= MS_PER_UPDATE)
		{
			update();
			lag -= MS_PER_UPDATE;
		}

		render();
	}
}

void Application::initWindow()
{
	m_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "TEST", sf::Style::Close | sf::Style::Titlebar);
	m_window.setFramerateLimit(144);
}

void Application::initVaribles()
{
	m_spaceShipVelocity    = 4.5f;
	m_bulletVelocity       = 4.f;
	m_enemyVelocity        = 1.f;
	m_enemySpawnTime	   = 1.f;
	m_enemyBulletSpawnTime = 2.f;

	m_spaceShipHealth      = 20;
	m_score				   = 0;
	m_enemiesKilled		   = 0;

	m_gameHasEnded		   = false;
	m_getTime			   = true;
}

void Application::initAssets()
{
	m_spaceShip = std::make_shared<Entity>("EspaceShip", m_assetManager.getTexture("TspaceShip"));
	m_spaceShip->getSprite().setScale(0.1f, 0.1f);
	m_spaceShip->getSprite().setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 1.5f);

	m_backgroundImage.setTexture(m_assetManager.getTexture("Tbackground"));
	m_backgroundImage.setScale(1.5f, 1.f);

	m_uiText.setFont(m_assetManager.getFont("SpaceFont"));
	m_uiText.setPosition(10.f, 10.f);
	m_uiText.setFillColor(sf::Color::Yellow);
	m_uiText.setCharacterSize(15);
}

void Application::loadAssets()
{
	m_assetManager.addFont	 ("SpaceFont", "../res/fonts/SPACE.ttf");
	m_assetManager.addTexture("TspaceShip", "../res/textures/spaceship.png");
	m_assetManager.addTexture("Tbullet", "../res/textures/bullet.png");
	m_assetManager.addTexture("Tenemy", "../res/textures/enemy.png");
	m_assetManager.addTexture("Tbackground", "../res/textures/background.png");
}

Entity Application::spawnBullet(sf::Vector2f pos, const std::string& name, sf::Color color)
{
	Entity entity(name, m_assetManager.getTexture("Tbullet"));
	entity.getSprite().setScale(0.1f, 0.1f);
	entity.getSprite().setPosition(pos); 
	entity.getSprite().setColor(color);
	return entity;
}

bool Application::checkCollision(std::shared_ptr<Entity>& e1, std::shared_ptr<Entity>& e2)
{
	return e1->getSprite().getGlobalBounds().intersects(e2->getSprite().getGlobalBounds());
}

void Application::sMovement()
{
	Vec2 playerVelocity;

	if (m_spaceShip->checkMove.left) {
		playerVelocity.x -= m_spaceShipVelocity;
	}
	if (m_spaceShip->checkMove.right) {
		playerVelocity.x += m_spaceShipVelocity;
	}
	if (m_spaceShip->checkMove.up) {
		playerVelocity.y -= m_spaceShipVelocity;
	}
	if (m_spaceShip->checkMove.down) {
		playerVelocity.y += m_spaceShipVelocity;
	}

	playerVelocity.normalize(); //Normalizing vector to ensure diagonal speed is same as normal speed
	playerVelocity *= m_spaceShipVelocity;

	m_spaceShip->getSprite().move(sf::Vector2f(playerVelocity.x, playerVelocity.y));
}
	
void Application::sCollision()
{
	//Checking side collisions (Note: -20 is the TextureRect error)
	if (m_spaceShip->getSprite().getPosition().y < 0) {
		m_spaceShip->getSprite().setPosition(m_spaceShip->getSprite().getPosition().x, 0); 
	}

	if (m_spaceShip->getSprite().getPosition().x < -20) {
		m_spaceShip->getSprite().setPosition(-20, m_spaceShip->getSprite().getPosition().y);
	}

	if (m_spaceShip->getSprite().getPosition().x + (m_spaceShip->getSprite().getGlobalBounds().width - 20) > SCREEN_WIDTH) {
		m_spaceShip->getSprite().setPosition(SCREEN_WIDTH - (m_spaceShip->getSprite().getGlobalBounds().width -20), m_spaceShip->getSprite().getPosition().y);
	}

	if (m_spaceShip->getSprite().getPosition().y + m_spaceShip->getSprite().getGlobalBounds().height > SCREEN_HEIGHT) {
		m_spaceShip->getSprite().setPosition(m_spaceShip->getSprite().getPosition().x, SCREEN_HEIGHT - m_spaceShip->getSprite().getGlobalBounds().height);
	}

	//Bullet and Enemy Collison Detection
	for (auto& b : m_bulletVec) {
		for (auto& e : m_enemyVec) {
			if (b->getName() == "SpaceBullet") {
				if (checkCollision(b, e)) {
					e->destroy();
					b->destroy();
					m_score += 10;
					m_enemiesKilled++;
				}
			}
		}
	}

	//SpaceShip and Enemy Collision Detection
	for (auto& e : m_enemyVec) {
		if (checkCollision(m_spaceShip, e)) {
			e->destroy();
			m_spaceShipHealth--;
			m_spaceShip->getSprite().setColor(sf::Color::Red);
			m_spaceShipColorClock.restart();
		}
	}
	
	//SpaceShip and Bullet Collision Detection
	for (auto& b : m_bulletVec) {
		if (b->getName() == "EnemyBullet") {
			if (checkCollision(m_spaceShip, b)) {
				b->destroy();
				m_spaceShipHealth--;
				m_spaceShip->getSprite().setColor(sf::Color::Red);
				m_spaceShipColorClock.restart();
			}
		}
	}
}

void Application::sShooting()
{
	for (auto& b : m_bulletVec)
	{
		if (b->getName() == "SpaceBullet") {
			b->getSprite().move(0, -m_bulletVelocity);

			if (b->getSprite().getPosition().y < 0) {
				b->destroy();
			}
		}

		if (b->getName() == "EnemyBullet") {
			b->getSprite().move(0.f, m_bulletVelocity - 1.f);

			if (b->getSprite().getPosition().y < 0) {
				b->destroy();
			}
		}
	}
}

void Application::sEnemySpawner()
{
	//spawns the enemy and random y pos from 0 to screenwidth
	if (m_enemyClock.getElapsedTime().asSeconds() > m_enemySpawnTime) {
		Entity e("Enemy", m_assetManager.getTexture("Tenemy"));

		e.getSprite().setPosition(rand() % static_cast<int>(SCREEN_WIDTH - e.getSprite().getGlobalBounds().width), 0.f);

		m_enemyVec.push_back(std::make_shared<Entity>(e));

		m_enemyClock.restart();
	}

	//Move the enemy and destroy it if it goes below screen height
	for (auto& e : m_enemyVec)
	{
		e->getSprite().move(0, m_enemyVelocity);

		if (e->getSprite().getPosition().y > SCREEN_HEIGHT) {
			e->destroy();
		}
	}

	//Just changes the color of ship back to normal after given seconds of colliding with enemy
	if (m_spaceShipColorClock.getElapsedTime().asSeconds() > 0.2f) {
		m_spaceShip->getSprite().setColor(sf::Color::White);
	}
}

void Application::sBulletSpawner()
{
	//SpaceShip bullet spawner
	if (m_spaceShip->checkMove.canShoot) {
		Entity e = spawnBullet(sf::Vector2f((m_spaceShip->getSprite().getPosition().x + (m_spaceShip->getSprite().getGlobalBounds().width / 2) - 8), m_spaceShip->getSprite().getPosition().y), "SpaceBullet");
		m_bulletVec.push_back(std::make_shared<Entity>(e));
		m_spaceShip->checkMove.canShoot = false;
	}

	//Enemy bullet spawner
	if (m_enemyBulletIntervalClock.getElapsedTime().asSeconds() > m_enemyBulletSpawnTime) {
		for (auto& e : m_enemyVec) {
			if (!e->isAlive()) { continue; }

			Entity bullet = spawnBullet(e->getSprite().getPosition(), "EnemyBullet",  sf::Color::Green);
			m_bulletVec.push_back(std::make_shared<Entity>(bullet));
			m_enemyBulletIntervalClock.restart();
		}
	}
}

void Application::sUI()
{
	std::stringstream ss;

	//Game UI 
	if (!m_gameHasEnded) {
		ss << "HEALTH =  " << m_spaceShipHealth << "\nSCORE =  " << m_score << "\nENEMIES KILLED =  " << m_enemiesKilled << "\nTime =  "
			<< std::floor(m_gameTimerClock.getElapsedTime().asSeconds() * 100.) / 100.;
	}

	//Game Over UI
	if (m_gameHasEnded) {
		m_uiText.setCharacterSize(30);
		m_uiText.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.75f);
		ss << "GAME OVER ! \n\n" << "SCORE =  " << m_score << "\nENEMIES KILLED =  " << m_enemiesKilled << "\nSurvival Time =  "
			<< m_endTime << "  seconds";
	}

	m_uiText.setString(ss.str());
}

void Application::sEndGame()
{
	if (m_spaceShipHealth <= 0) {
		m_gameHasEnded = true;
	}

	if (m_gameHasEnded && m_getTime) {
		m_endTime = std::floor(m_gameTimerClock.getElapsedTime().asSeconds() * 100.) / 100.;
		m_getTime = false;
	}
}

void Application::handleInput()
{
	//No logic in input handler . Makes it easy to change keybinds on the go
	sf::Event e;
	while (m_window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			m_window.close();
		}

		if (e.type == sf::Event::KeyPressed)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::W:
				m_spaceShip->checkMove.up    = true;
				break;
			case sf::Keyboard::A:
				m_spaceShip->checkMove.left  = true;
				break;
			case sf::Keyboard::S:
				m_spaceShip->checkMove.down  = true;
				break;
			case sf::Keyboard::D:
				m_spaceShip->checkMove.right = true;
				break;
			case sf::Keyboard::Space:
				m_spaceShip->checkMove.canShoot = true;
				break;
			case sf::Keyboard::Escape:
				m_window.close();
				break;
			}
		}

		if (e.type == sf::Event::KeyReleased)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::W:
				m_spaceShip->checkMove.up    = false;
				break;
			case sf::Keyboard::A:
				m_spaceShip->checkMove.left  = false;
				break;
			case sf::Keyboard::S:
				m_spaceShip->checkMove.down  = false;
				break;
			case sf::Keyboard::D:
				m_spaceShip->checkMove.right = false;
				break;
			case sf::Keyboard::Space:
				m_spaceShip->checkMove.canShoot = false;
				break;
			}
		}
	}
}

void Application::update()
{
	if (!m_gameHasEnded) {
		sMovement();
		sCollision();
		sShooting();
		sEnemySpawner();
		sBulletSpawner();
	}

	sUI();
	sEndGame();

	//Not the fastest method but its simple and it works so yay
	m_enemyVec.erase(std::remove_if(m_enemyVec.begin(), m_enemyVec.end(),
		[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }), m_enemyVec.end());
	
	m_bulletVec.erase(std::remove_if(m_bulletVec.begin(), m_bulletVec.end(),
		[](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }), m_bulletVec.end());
}

void Application::render()
{
	m_window.clear(sf::Color::Black);

	//DRAW HERE
	m_window.draw(m_backgroundImage);
	m_window.draw(m_uiText);

	if (!m_gameHasEnded) {
		if (m_spaceShip->isAlive()) {
			m_window.draw(m_spaceShip->getSprite());
		}

		for (auto& b : m_bulletVec)
		{
			if (b->isAlive()) {
				m_window.draw(b->getSprite());
			}
		}

		for (auto& e : m_enemyVec)
		{
			if (e->isAlive()) {
				m_window.draw(e->getSprite());
			}
		}
	}
	//---------

	m_window.display();
}
