#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1EntityManager.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "p2Animation.h"

struct SDL_Texture;
struct Collider;
struct Animation;

enum ENTITY_TYPE;

class j1Entity
{
public:
	j1Entity(int x, int y, ENTITY_TYPE type);
	j1Entity() {}

	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

public:

	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&);

	virtual void OnCollision(Collider* c1, Collider* c2) {};

	ENTITY_TYPE type;

	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprites = nullptr;

};

#endif // __ENTITY_H__