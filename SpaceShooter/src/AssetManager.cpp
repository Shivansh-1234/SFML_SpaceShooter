#include "AssetManager.h"

AssetManager::AssetManager()
{
}

void AssetManager::addTexture(const std::string& textureName, const std::string& filePath, bool smooth)
{
	m_textureMap[textureName] = sf::Texture();

	if (!m_textureMap[textureName].loadFromFile(filePath)) {
		std::cout << "E: FAILED TO LOAD TEXTURE : " << filePath << std::endl;
		m_textureMap.erase(textureName);
		return;
	}

	m_textureMap[textureName].setSmooth(smooth);
}

void AssetManager::addFont(const std::string& fontName, const std::string& filePath)
{
	m_fontMap[fontName] = sf::Font();

	if (!m_fontMap[fontName].loadFromFile(filePath)) {
		std::cout << "E: FAILED TO LOAD FONT : " << filePath << std::endl;
		m_fontMap.erase(fontName);
		return;
	}
}

const sf::Texture& AssetManager::getTexture(const std::string& textureName)
{
	assert(m_textureMap.find(textureName) != m_textureMap.end());

	return m_textureMap.at(textureName);
}

const sf::Font& AssetManager::getFont(const std::string& fontName)
{
	assert(m_fontMap.find(fontName) != m_fontMap.end());

	return m_fontMap.at(fontName);
}
