#pragma once

#include "DEPENDENCIES.h"

class AssetManager
{
private:
	std::map <std::string, sf::Texture> m_textureMap;
	std::map <std::string, sf::Font>    m_fontMap;

public:
	AssetManager();

	void addTexture(const std::string& textureName, const std::string& filePath, bool smooth = true);
	void addFont(const std::string& fontName, const std::string& filePath);

	const sf::Texture& getTexture(const std::string& textureName);
	const sf::Font&    getFont(const std::string& fontName);
};