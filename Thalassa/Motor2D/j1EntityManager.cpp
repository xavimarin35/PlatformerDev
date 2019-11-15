#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Chest.h"
#include "j1MovingObstacle.h"
#include "j1Door.h"
#include "j1Bat.h"
#include "j1Demon.h"
#include "j1BatEnemy.h"
#include "j1LifeItem.h"
#include "j1JetpackItem.h"

j1EntityManager::j1EntityManager()
{
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node &)
{
	return true;
}

bool j1EntityManager::Start()
{
	for (p2List_item<j1Entity*>* it = entityList.start; it != nullptr; it = it->next)
	{
		it->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPE::NONE) 
		{
			SpawnEnemy(queue[i]);
			queue[i].type = ENTITY_TYPE::NONE;
		}
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (p2List_item<j1Entity*>* it = entityList.start; it != nullptr; it = it->next)
	{
		it->data->Update(dt);
	}
	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (p2List_item<j1Entity*>* it = entityList.start; it != nullptr; it = it->next)
	{
		it->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	for (p2List_item<j1Entity*>* it = entityList.end; it != NULL; it = it->prev)
	{
		ret = it->data->CleanUp();
	}

	entityList.clear();
	player = nullptr;

	return ret;
}

void j1EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	for (p2List_item<j1Entity*>* it = entityList.start; it != nullptr; it = it->next) 
	{
		if (it->data->collider == c1)
		{
			it->data->OnCollision(c1, c2);
			it->data->OnCollision(c2, c1);
			break;
		}
	}
}

j1Entity* j1EntityManager::EntityFactory(ENTITY_TYPE type, float x, float y)
{
	j1Entity* ret = nullptr;

	switch (type) 
	{
	case ENTITY_TYPE::PLAYER:
		ret = new j1Player(x, y, type);

		if (ret != nullptr)
			entityList.add(ret);
		break;

	case ENTITY_TYPE::CHEST:
		ret = new j1Chest(x, y, type);

		if (ret != nullptr)
			entityList.add(ret);
		break;

	case ENTITY_TYPE::DOOR:
		ret = new j1Door(x, y, type);

		if (ret != nullptr)
			entityList.add(ret);
		break;

	case ENTITY_TYPE::BAT:
		ret = new j1Bat(x, y, type);

		if (ret != nullptr)
			entityList.add(ret);
		break;
	case ENTITY_TYPE::LIFE_ITEM:
		ret = new j1LifeItem(x, y, type);

		if (ret != nullptr)
			entityList.add(ret);
		break;
	}

	return ret;
}

void j1EntityManager::CreateEntity(ENTITY_TYPE type, float x, float y)
{
	switch (type) {
		case ENTITY_TYPE::PLAYER:
			player = (j1Player*)EntityFactory(ENTITY_TYPE::PLAYER, x, y);
			break;

		case ENTITY_TYPE::CHEST:
			chest = (j1Chest*)EntityFactory(ENTITY_TYPE::CHEST, x, y);
			break;

		case ENTITY_TYPE::DOOR:
			door = (j1Door*)EntityFactory(ENTITY_TYPE::DOOR, x, y);
			break;

		case ENTITY_TYPE::BAT:
			bat = (j1Bat*)EntityFactory(ENTITY_TYPE::BAT, x, y);
			break;
		
		case ENTITY_TYPE::LIFE_ITEM:
			lifeItem = (j1LifeItem*)EntityFactory(ENTITY_TYPE::LIFE_ITEM, x, y);
			break;

	}
}

void j1EntityManager::AddEnemy(float x, float y, ENTITY_TYPE type)
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPE::NONE)
		{
			queue[i].type = type;
			queue[i].position.x = x;
			queue[i].position.y = y;
			break;
		}
	}
}

void j1EntityManager::SpawnEnemy(const EntityInfo & info)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPE::NONE)
		{
			j1Entity* ret = nullptr;

			switch (info.type) {
			case ENTITY_TYPE::OBSTACLE:
				ret = new j1MovingObstacle(info.position.x, info.position.y, info.type);

				if (ret != nullptr)
					entityList.add(ret);
				break;

			case ENTITY_TYPE::DEMON:
				ret = new j1Demon(info.position.x, info.position.y, info.type);

				if (ret != nullptr)
					entityList.add(ret);
				break;

			case ENTITY_TYPE::BAT_E:
				ret = new j1BatEnemy(info.position.x, info.position.y, info.type);

				if (ret != nullptr)
					entityList.add(ret);
				break;
			}

			ret->Start();

			break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node &)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node &) const
{
	return true;
}
