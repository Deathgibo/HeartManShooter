#pragma once
#include "Model.h"
#include <vector>
#include <SDL_mixer.h>

struct BulletInfo {
	Model bulletmodels;
	glm::mat4 bulletmatrix;
	bool firsttime;
	int bulletlifetime;
	int bulletlifetimecounter;
	int bulletangle;
	int randomincrement;

	BulletInfo(Model model, glm::mat4 matrix)
	{
		bulletmodels = model;
		bulletmodels.copy = true;
		bulletmatrix = matrix;
		firsttime = true;

		bulletlifetimecounter = 0;
		bulletlifetime = 200;
		bulletangle = 0;
	}
};
class Weapon
{
public:
	Weapon();
	void CreateAk47();
	void CreateAwp();
	void CreatePistol();

	void SpawnBullet();
	void UpdateBullet(int i);
	void RenderWeapon() { model.RenderModel(); }
	~Weapon();

	bool automatic;

	int weaponID;
	Model model;
	glm::vec3 Offset;
	glm::vec3 AimOffset;
	glm::vec3 aimvector;
	glm::vec3 CurrentOffset;
	glm::vec3 BulletOffset;
	glm::vec3 FlashOffset;
	glm::vec3 RayOffset;
	//damage, sound, reload time, etc...
	int maxammo;
	int currentammo;
	int reloadcounter;
	int reloadtime;
	int firerate;
	int fireratecounter;

	std::vector<BulletInfo> Bullets;


	/*Sounds*/
	Mix_Music* firingsound;
	Mix_Chunk* reloadsound;
	Mix_Chunk* firingsoundchunk;

private:
};

