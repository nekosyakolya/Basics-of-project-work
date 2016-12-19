#include "stdafx.h"
#include "level.h"

int Object::GetPropertyInt(std::string name)//возвращаем номер свойства в нашем списке
{
	return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
	return static_cast<float>(strtod(properties[name].c_str(), NULL));
}

std::string Object::GetPropertyString(std::string name)
{
	return properties[name];
}

bool Level::LoadFromFile(const std::string &filename)
{
	TiXmlDocument levelFile(filename.c_str());//загружаем файл в TiXmlDocument

	if (!levelFile.LoadFile())//если не удалось загрузить карту
	{
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;//выдаем ошибку
		return false;
	}

	// работаем с контейнером map
	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");



	m_width = atoi(map->Attribute("width"));//извлекаем из нашей карты ее свойства
	m_height = atoi(map->Attribute("height"));//те свойства, которые задавали при работе в 
	m_tileWidth = atoi(map->Attribute("tilewidth"));//тайлмап редакторе
	m_tileHeight = atoi(map->Attribute("tileheight"));

	// Берем описание тайлсета и идентификатор первого тайла
	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	m_firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - путь до картинки в контейнере image
	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// пытаемся загрузить тайлсет
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;//если не удалось загрузить тайлсет-выводим ошибку в консоль
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	m_tilesetImage.loadFromImage(img);
	m_tilesetImage.setSmooth(false);//сглаживание

	int columns = m_tilesetImage.getSize().x / m_tileWidth;
	int rows = m_tilesetImage.getSize().y / m_tileHeight;

	// вектор из прямоугольников изображений (TextureRect)
	std::vector<sf::IntRect> subRects;

	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < columns; ++x)
		{
			sf::IntRect rect;

			rect.top = y * m_tileHeight;
			rect.height = m_tileHeight;
			rect.left = x * m_tileWidth;
			rect.width = m_tileWidth;

			subRects.push_back(rect);
		}

	// работа со слоями
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;

		// если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if (layerElement->Attribute("opacity") != NULL)
		{
			double opacity = strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = static_cast<int>(255 * opacity);
		}
		else
		{
			layer.opacity = 255;
		}

		//  контейнер <data> 
		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  контейнер <tile> - описание тайлов каждого слоя
		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - m_firstTileID;

			// Устанавливаем TextureRect каждого тайла
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(m_tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(static_cast<float>(x * m_tileWidth), static_cast<float>(y * m_tileHeight));
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

				layer.tiles.push_back(sprite);	//закидываем в слой спрайты тайлов
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= m_width)
			{
				x = 0;
				y++;
				if (y >= m_height)
					y = 0;
			}
		}

		m_layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// работа с объектами
	TiXmlElement *objectGroupElement;

	// если есть слои объектов
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			//  контейнер <object>
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// получаем все данные - тип, имя, позиция, и тд
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				float x = static_cast <float>(atoi(objectElement->Attribute("x")));
				float y = static_cast <float>(atoi(objectElement->Attribute("y")));

				int width;
				int height;

				sf::Sprite sprite;
				sprite.setTexture(m_tilesetImage);
				sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - m_firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - m_firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - m_firstTileID]);
				}

				// экземпляр объекта
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::FloatRect objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = static_cast <float>(height);
				objectRect.width = static_cast <float>(width);
				object.rect = objectRect;

				// "переменные" объекта
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement *prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				m_objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}

Object Level::GetObject(const std::string &name)const
{
	Object obj = {};
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i].name == name)
		{
			obj = m_objects[i];
		}
	}
	return obj;

}

std::vector<Object> Level::GetObjects(const std::string &name)const
{
	std::vector<Object> vec;

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i].name == name)
		{
			vec.push_back(m_objects[i]);
		}

	}
	return vec;
}


std::vector<Object> Level::GetAllObjects()const
{
	return m_objects;
};


sf::Vector2i Level::GetTileSize()const
{
	return sf::Vector2i(m_tileWidth, m_tileHeight);
}



void Level::Draw(sf::RenderTarget &target)const
{
	const sf::FloatRect viewportRect = target.getView().getViewport();

	// Draw all tiles (and don't draw objects)
	for (const auto &layer : m_layers)
	{
		for (const auto &tile : layer.tiles)
		{
			if (viewportRect.intersects(tile.getLocalBounds()))
			{
				target.draw(tile);
			}
		}
	}
}

