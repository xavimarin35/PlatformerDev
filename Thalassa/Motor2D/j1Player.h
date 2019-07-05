#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Player : public j1Module
{
public:

	j1Player();
	virtual ~j1Player();

	//bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:


public:
	iPoint position;	
	bool dead = false;
};

#endif // __jPLAYER_H__