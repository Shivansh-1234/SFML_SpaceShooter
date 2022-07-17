#pragma once
#include "DEPENDENCIES.h"

struct Movement {
	bool left  = false;
	bool right = false;
	bool up    = false;
	bool down  = false;
	bool canShoot = false;
};

class Entity
{
private:
	std::string m_name = "NONE";
	bool	    m_isalive = true;
	sf::Sprite  m_sprite;

public:
	Entity();
	Entity(const std::string& name, const sf::Texture& texture, bool isAlive = true);

	Movement checkMove;

	void destroy();

	const bool isAlive() const;
	const std::string& getName() const;
	sf::Sprite& getSprite();
};
