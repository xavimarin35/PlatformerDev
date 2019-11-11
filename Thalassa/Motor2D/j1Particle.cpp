#include "j1App.h"
#include "j1Particle.h"
#include "j1Map.h"
#include "j1Scene1.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Player.h"
#include "p2Animation.h"
#include "j1Scene1.h"
#include "j1Entity.h"
#include "j1Audio.h"


j1Particle::j1Particle()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	//basic
	basicShoot.anim.PushBack({ 65,75,8,6 });
	basicShoot.anim.PushBack({ 77,74,8,7 });
	basicShoot.anim.speed = 0.1f;
	basicShoot.life = 2000;
	basicShoot.type = BASIC_SHOOT;

	basicShootDestroyed.anim.PushBack({ 88,73,9,9 });
	basicShootDestroyed.anim.PushBack({ 100,73,12,9 });
	basicShootDestroyed.anim.PushBack({ 113,73,12,11 });
	basicShootDestroyed.anim.PushBack({ 129,71,14,12 });
	basicShootDestroyed.anim.speed = 0.1f;
	basicShootDestroyed.life = 200;

	//remote
	remoteShoot.anim.PushBack({ 52,25,11,6 });
	remoteShoot.anim.PushBack({ 70,24,9,7 });
	remoteShoot.anim.speed = 0.1f;
	remoteShoot.life = 2000;
	remoteShoot.type = REMOTE_SHOOT;

	remoteShootDestroyed.anim.PushBack({ 83,19,11,17 });
	remoteShootDestroyed.anim.PushBack({ 103,17,14,21 });
	remoteShootDestroyed.anim.PushBack({ 125,17,14,19 });
	remoteShootDestroyed.anim.speed = 0.1f;
	remoteShootDestroyed.life = 200;

	//demon
	demonShoot.anim.PushBack({ 117,184,9,3 });
	demonShoot.anim.PushBack({ 153,184,5,3 });
	demonShoot.anim.speed = 0.1f;
	demonShoot.life = 2000;
	demonShoot.type = DEMON_SHOOT;

	demonShootDestroyed.anim.PushBack({ 186,181,4,8 });
	demonShootDestroyed.anim.PushBack({ 153,184,5,3 });
	demonShootDestroyed.anim.speed = 0.1f;
	demonShootDestroyed.life = 200;


}

j1Particle::~j1Particle()
{}

// Load assets
bool j1Particle::Start()
{
	LOG("Loading particles");
	part2_tex = App->tex->Load("textures/Particles/shots.png");
	demonShot_tex = App->tex->Load("textures/Enemies/Demon_Spritesheet.png");

	basicShoot.tex = part2_tex;
	basicShootDestroyed.tex = part2_tex;
	remoteShoot.tex = part2_tex;
	remoteShootDestroyed.tex = part2_tex;
	demonShoot.tex = demonShot_tex;
	demonShootDestroyed.tex = demonShot_tex;


	return true;
}

bool j1Particle::CleanUp()
{
	LOG("Unloading particles");
	App->tex->UnLoad(part2_tex);
	App->tex->UnLoad(demonShot_tex);

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			if (active[i]->type == BASIC_SHOOT)
			{
				App->tex->UnLoad(part2_tex);
				delete active[i];
				active[i] = nullptr;
			}
		}
	}
	return true;
}

bool j1Particle::Update(float dt)
{
		for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		{
			Particle* p = active[i];

			if (p == nullptr)
				continue;

			if (p->Update(dt) == false)
			{
				delete p;
				active[i] = nullptr;
			}
			else if (SDL_GetTicks() >= p->born)
			{
				App->render->Blit(p->tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()), SDL_FLIP_NONE, 1.0F, p->rotation);

				if (p->fx_played == false)
				{
					p->fx_played = true;
				}
			}
		}

	return true;
}

void j1Particle::AddParticle(const Particle& particle, int x, int y, float dt, COLLIDER_TYPE collider_type, Uint32 delay, double rotation, PARTICLE_TYPE ptype)
{

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->rotation = rotation;
			p->type = ptype;
			p->state = 0;
			p->anim.Reset();
			if (collider_type == COLLIDER_SHOOT) {

				switch (particle.type)
				{
				case PARTICLE_TYPE::BASIC_SHOOT:
					p->collider = App->collisions->AddCollider({ p->anim.GetCurrentFrame().x, p->anim.GetCurrentFrame().y, 5, 5 }, collider_type, this);
					break;
				case PARTICLE_TYPE::REMOTE_SHOOT:
					p->collider = App->collisions->AddCollider({ p->anim.GetCurrentFrame().x, p->anim.GetCurrentFrame().y, 5, 5 }, collider_type, this);
					break;
				}
				
			}

			Collider* test = p->collider;
			active[i] = p;
			break;
		}
	}
}

void j1Particle::OnCollision(Collider* c1, Collider* c2)
{
	bool particleDestroyed = false;
		if (c2->type == COLLIDER_ENEMY) 
		{
			int ret = true;
			for (uint i = 0; i < MAX_ACTIVE_PARTICLES && ret; ++i)
			{
				if (active[i] != nullptr) {
					if (active[i]->collider == c1) {
						switch (active[i]->type) {
						case BASIC_SHOOT:
								AddParticle(demonShootDestroyed, active[i]->position.x, active[i]->position.y, 0);
								particleDestroyed = true;
							break;
						case REMOTE_SHOOT:
								AddParticle(demonShootDestroyed, active[i]->position.x, active[i]->position.y, 0, COLLIDER_SHOOT, 0, (double)-90.0f, REMOTE_SHOOT);
								particleDestroyed = true;
							break;
						}
						ret = false;
					}
				}
			}
		}
		
	
// destroy the particle collided
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr && active[i]->collider == c1 && particleDestroyed)
		{
			delete active[i];
			active[i] = nullptr;
			break;
		}

	}
	

}

// -------------------------------------------------------------
// -------------------------------------------------------------
Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), type(p.type), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life), tex(p.tex)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life) {
			ret = false;	
		}
	}

	position.x += speed.x/* * dt*/;
	position.y += speed.y/* * dt*/;
	
	if (collider != nullptr) 
	{
		collider->SetPos(position.x, position.y);
	}

	return ret;
}