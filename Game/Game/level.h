#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "tinyxml.h"

struct Object
{
	int GetPropertyInt(std::string name);
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;
	std::string type;
	sf::FloatRect rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

struct Layer
{
	int opacity;
	std::vector<sf::Sprite> tiles;
};

class Level
{
public:

	bool LoadFromFile(const std::string &filepath);

	Object GetObject(const std::string &name)const;
	std::vector<Object> GetObjects(const std::string &name)const;
	std::vector<Object> GetAllObjects()const;
	sf::Vector2i GetTileSize()const;

	void Draw(sf::RenderTarget &target)const;

private:
	int m_width = 0;
	int m_height = 0;
	int m_tileWidth = 0;
	int m_tileHeight = 0;
	int m_firstTileID = 0;
	sf::Texture m_tilesetImage;
	std::vector<Object> m_objects;
	std::vector<Layer> m_layers;
};


#endif

