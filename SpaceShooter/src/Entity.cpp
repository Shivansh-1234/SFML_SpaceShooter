#include "Entity.h"

Entity::Entity() 
{ }

Entity::Entity(const std::string& name, const sf::Texture& texture, bool isAlive)
	:m_name(name)
	,m_isalive(isAlive)
{
	m_sprite.setTexture(texture);
}

void Entity::destroy()
{
	m_isalive = false;
}

sf::Sprite& Entity::getSprite()
{
	return m_sprite;
}

const std::string& Entity::getName() const
{
	return m_name;
}

const bool Entity::isAlive() const
{
	return m_isalive;
}

