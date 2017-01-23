#include "stdafx.h"
#include "MiniGame.h"



using namespace sf;
namespace
{
	std::map <unsigned, std::string> LevelMap = { { 1, "resources/mission1.tmx" },{ 2, "resources/mission.tmx" } };
}

CMiniGame::CMiniGame()
{
}

CMiniGame::~CMiniGame()
{
}

void CMiniGame::CheckCollision(float time)
{
	sf::Vector2f offset;
	for (size_t i = 0; i < m_boxes.size(); ++i)
	{
		if (m_player.GetRect().intersects(m_boxes[i]->sprite.getGlobalBounds()))
		{
			m_player.offset.x *= time;
			m_player.offset.y *= time;
			if (m_player.offset.x < 0)
			{
				offset.x = -SIZE_TILE;
				offset.y = 0;
			}
			if (m_player.offset.x > 0)
			{
				offset.x = SIZE_TILE;
				offset.y = 0;
			}
			if (m_player.offset.y > 0)
			{

				offset.x = 0;
				offset.y = SIZE_TILE;
			}

			if (m_player.offset.y < 0)
			{
				offset.x = 0;
				offset.y = -SIZE_TILE;
			}
			CheckObjects(i, offset);
		}


		auto positionSprite = (m_boxes[i]->isStateFinal) ? sf::Vector2f(0, 116) : sf::Vector2f(0, 58);

		m_boxes[i]->sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	}
}

void CMiniGame::Init()
{
	m_level = 1;

	Initialisation();

	m_next = false;

	m_font.loadFromFile("resources/CyrilicOld.TTF");

	m_text.setCharacterSize(46);
	m_text.setFont(m_font);
	m_text.setString("");
	m_text.setFillColor(sf::Color::Yellow);
	m_text.setStyle(sf::Text::Bold);
	m_text.setPosition(400, 0);
	m_textMission.setCharacterSize(25);
	m_textMission.setFont(m_font);

	m_textMission.setFillColor(sf::Color::Black);
	m_textMission.setStyle(sf::Text::Bold);
	m_textMission.setPosition(90, 110);

	m_texture.loadFromFile("resources/fon.jpg");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(0, 0);
	m_sprite.setScale(sf::Vector2f(0.6f, 0.6f));

	m_textureButton.loadFromFile("resources/play.png");
	m_spriteButton.setTexture(m_textureButton);
	m_spriteButton.setPosition(270, 330);
	m_spriteButton.setScale(sf::Vector2f(0.5f, 0.5f));

	m_textureRestart.loadFromFile("resources/restart.png");
	m_spriteRestart.setTexture(m_textureRestart);
	m_spriteRestart.setPosition(10, 10);
	m_spriteRestart.setScale(sf::Vector2f(0.2f, 0.2f));

	m_buffer.loadFromFile("resources/mission.wav");
	m_sound.setBuffer(m_buffer);
	m_sound.setLoop(true);
	m_sound.setVolume(60);
}



void CMiniGame::SetString()
{
	UpdateTime();
	m_text.setString("Время: " + std::to_string(m_gameTime));

}

Level CMiniGame::GetMap() const
{
	return m_map;
}

Text CMiniGame::GetText() const
{
	return m_text;
}

bool CMiniGame::IsNext() const
{
	return m_next;
}

sf::Sprite CMiniGame::GetSprite() const
{
	return m_sprite;
}

sf::Text CMiniGame::GetTextMission() const
{
	return m_textMission;
}

sf::Sprite CMiniGame::GetSpriteMission() const
{
	return m_spriteButton;
}

sf::Sprite CMiniGame::GetRestart() const
{
	return m_spriteRestart;
}

void CMiniGame::SetNext()
{
	m_gameTimeClock.restart();
	m_next = true;
}

void CMiniGame::ChangeButton()
{
	m_textureButton.loadFromFile("resources/next.png");
	m_spriteButton.setTexture(m_textureButton);

}

bool CMiniGame::hasCollisionWithBox(const size_t & boxNo, const sf::Vector2f &offset)
{
	bool isCollision = false;
	m_boxes[boxNo]->sprite.setPosition(m_boxes[boxNo]->sprite.getPosition().x + offset.x, m_boxes[boxNo]->sprite.getPosition().y + offset.y);
	for (size_t j = 0; j < m_boxes.size(); ++j)
	{
		auto &box1 = m_boxes[boxNo];
		auto &box2 = m_boxes[j];
		if (m_boxes[boxNo]->sprite.getPosition() == m_boxes[j]->sprite.getPosition() && (&box1 != &box2))
		{
			ChangePosition(*(m_boxes[boxNo]), offset);
			isCollision = true;
		}
	}
	return isCollision;
}

void CMiniGame::CheckObjects(const size_t & boxNo, const sf::Vector2f &offset)
{
	bool isCollision = hasCollisionWithBox(boxNo, offset);

	for (size_t j = 0; j < m_player.obj.size(); ++j)
	{
		auto &solidObject = m_player.obj[j];

		if ((solidObject.rect.intersects((*m_boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "collision"))
		{
			ChangePosition(*(m_boxes[boxNo]), offset);
			isCollision = true;
			break;
		}

		if ((solidObject.rect.intersects((*m_boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "goal"))
		{

			if (!m_boxes[boxNo]->isStateFinal)
			{
				m_boxes[boxNo]->isStateFinal = true;
			}

			isCollision = true;
		}
	}
	if (!isCollision && m_boxes[boxNo]->isStateFinal)
	{
		m_boxes[boxNo]->isStateFinal = false;
	}
}

void CMiniGame::ChangePosition(Box & box, const sf::Vector2f & offset)
{
	m_player.position.x -= m_player.offset.x;
	m_player.position.y -= m_player.offset.y;
	m_player.direction = PlayerDirection::STAY;
	box.sprite.setPosition(box.sprite.getPosition().x + (-offset.x), box.sprite.getPosition().y + (-offset.y));
}

void CMiniGame::UpdateTime()
{
	m_gameTime = static_cast<unsigned>(m_gameTimeClock.getElapsedTime().asSeconds());
}

void CMiniGame::Initialisation()
{
	m_map.LoadFromFile(LevelMap.find(m_level)->second);
	m_hero = m_map.GetObject("player");

	m_player.InitPlayer(m_map, Vector2f(m_hero.rect.left, m_hero.rect.top));

	std::vector<Object> boxesStartPos = m_map.GetObjects("box");

	while (!m_boxes.empty())
	{
		m_boxes.erase(m_boxes.end() - 1);
	}

	bool isFinal;
	for (auto currBox : boxesStartPos)
	{
		isFinal = (currBox.type == "boxFinal") ? true : false;
		m_boxes.push_back(std::shared_ptr<Box>(new Box(Vector2f(currBox.rect.left, currBox.rect.top), isFinal)));
	}


	m_gameTime = 0;
	m_time = 0;
	m_gameTimeClock.restart();


	m_textureButton.loadFromFile("resources/play.png");
	m_spriteButton.setTexture(m_textureButton);



	m_player.direction = PlayerDirection::STAY;
	m_textMission.setString(
		"Вы попались!\nВремя гонок заморожено.\nИ теперь вам придется расставить ящики,\nчтобы продолжить гонку и стать лидером.\nУ вас есть всего лишь 90 секунд!\nУспели-ваша скорость возрастет,\nиначе-упадет.");



}

unsigned CMiniGame::GetTimePlay() const
{
	return m_gameTime;
}

unsigned CMiniGame::GetTimeDelay() const
{
	return m_time;
}

void CMiniGame::LevelUp()
{
	if ((--LevelMap.end())->first == m_level)
	{
		m_level = 1;
	}
	else
	{
		++m_level;
	}
	m_next = false;

}

void CMiniGame::SetTimeDelay()
{
	++m_time;
}

void CMiniGame::SetText(const std::string & newString)
{
	m_textMission.setString(newString);
}

void CMiniGame::PlaySound()
{
	m_sound.play();
}

void CMiniGame::StopSound()
{
	m_sound.stop();
}



void Player::GetDirection()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		direction = PlayerDirection::LEFT;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		direction = PlayerDirection::RIGHT;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up)))
	{
		direction = PlayerDirection::UP;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		direction = PlayerDirection::DOWN;
	}
}

void Player::Update(float time)
{
	currentFrame += 0.005f * time;
	if (direction == PlayerDirection::STAY)
	{
		GetDirection();
	}

	if (currentFrame > 2 && (direction == PlayerDirection::RIGHT || direction == PlayerDirection::LEFT))
	{
		currentFrame = 0;
	}
	if (currentFrame > 3 && (direction == PlayerDirection::UP || direction == PlayerDirection::DOWN))
	{
		currentFrame = 0;
	}

	switch (direction)
	{
	case PlayerDirection::RIGHT:
		offset.x = 0.1f;
		offset.y = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case PlayerDirection::LEFT:
		offset.x = -0.1f;
		offset.y = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER * 3, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case PlayerDirection::UP:
		offset.y = -0.1f;
		offset.x = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER * 2, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case PlayerDirection::DOWN:
		offset.y = 0.1f;
		offset.x = 0;
		sprite.setTextureRect(IntRect(0, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case PlayerDirection::STAY:
		offset.x = 0;
		offset.y = 0;
		break;
	default:
		break;
	}
	if (offset.x != 0)
	{
		position.x += offset.x * time;
		CheckCentre(position.x);
		CheckCollision(offset.x, 0);
	}
	if (offset.y != 0)
	{
		position.y += offset.y * time;
		CheckCentre(position.y);
		CheckCollision(0, offset.y);
	}
}

void Player::InitPlayer(Level & level, const Vector2f &positionPlayer)
{
	obj = level.GetAllObjects();

	position = Vector2f(positionPlayer.x + (SIZE_TILE / 2), positionPlayer.y + (SIZE_TILE / 2));
	currentFrame = 0.0;

	offset.x = 0.0;
	offset.y = 0.0;
	image.loadFromFile("resources/player1.png");
	texture.loadFromImage(image);

	sprite.setOrigin((WIDTH_PLAYER / 2), (SIZE_TILE / 2));
	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(0, 0, WIDTH_PLAYER, SIZE_TILE));

	sprite.setPosition(position);

}

sf::FloatRect Player::GetRect()
{
	return FloatRect(position.x - (SIZE_TILE / 2), position.y - (SIZE_TILE / 2), 57, 57);

}

void Player::CheckCollision(const float & dx, const float & dy)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		if (GetRect().intersects(obj[i].rect) && (obj[i].name == "collision"))
		{
			direction = PlayerDirection::STAY;
			if (dy > 0)
			{
				position.y = obj[i].rect.top - (SIZE_TILE / 2);
			}
			if (dy < 0)
			{
				position.y = obj[i].rect.top + obj[i].rect.height + (SIZE_TILE / 2);
			}
			if (dx > 0)
			{
				position.x = obj[i].rect.left - (SIZE_TILE / 2);
			}
			if (dx < 0)
			{
				position.x = obj[i].rect.left + obj[i].rect.width + (SIZE_TILE / 2);
			}
		}
	}
}

void Player::CheckCentre(const float & value)
{
	if (static_cast<int>(value - (SIZE_TILE / 2)) % SIZE_TILE == 0)
	{
		direction = PlayerDirection::STAY;
	}
}

Box::Box(const sf::Vector2f & position, bool isState) :
	position(position),
	isStateFinal(isState)
{
	image.loadFromFile("resources/sprite.png");

	auto positionSprite = (isStateFinal) ? Vector2f(0, 116) : Vector2f(0, 58);
	texture.loadFromImage(image);

	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	sprite.setPosition(position);

}
