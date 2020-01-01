#pragma once
#include "Weapon.h"
#include "Shader.h"
#include "Camera.h"
#include "HitPlane.h"
#include "Utils.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Material.h"
#include "ShadowMap.h"
#include <SDL_mixer.h>
#include <glm/gtc/type_ptr.hpp>

struct BulletHole {
	Rectangle rect;
	int timer;

	BulletHole(glm::vec3 norm, glm::vec3 left, glm::vec3 up, glm::vec3 acenter, float thewidth, float theheight, float thedepth = 0)
	{
		rect.GenerateFlatRectangleInTriangle(norm, left, up, acenter, thewidth, theheight);
		rect.center = acenter;

		timer = 0;
	}
};

class Player
{
public:
	Player();
	Player(Model* body, Weapon* gun);

	void Initialize(Model* body, Weapon* gun);
	void RenderPlayer(Shader* theshader, bool shadowpass = false);
	void PlayerUpdate(Singlekeys* singlekeys);
	void HandleInput(bool * keys);
	void SwapWeapon();
	void MovePlayer();
	~Player();


	Model* model_body;
	glm::vec3 model_bodyOffset;
	Weapon* weapon;

	HitBox hitbox;
	glm::vec3 hitboxOffset;
	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec3 jumpvector;
	bool feetonground;
	
	float movespeed;

	/*aiming*/
	//bullet array
	int aimcounter = 0;
	bool reloading;
	bool shooting;
	bool canshootagain;
	bool canaimagain;
	bool kickback;
	float kickbackpercent;
	bool kickbackdir;
	bool flashon;
	int flashoncounter = 0;
	int flashrotation = 0;
	bool shootray;
	vector<BulletHole*> bulletholes;
	bool weaponscoped = false;
private:
	void HandlePhysics();
	void kickbackroutine();
	void Shootray();

};

