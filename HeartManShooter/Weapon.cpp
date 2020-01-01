#include "Weapon.h"

Weapon::Weapon()
{

}

void Weapon::CreateAk47()
{
	extern Mix_Music* sound_akfiring;
	extern Mix_Chunk* sound_akreload;

	weaponID = 0;
	firingsound = sound_akfiring;
	reloadsound = sound_akreload;
	automatic = true;

	maxammo = 30;
	currentammo = 30;
	reloadcounter = 0;
	reloadtime = 200;
	fireratecounter = 0;
	firerate = 10;
	//Offset = glm::vec3(1.2f, -2.0f, -4.3f); //without model
	Offset = glm::vec3(0.8f, -1.5f, 3.5f); //with model
	AimOffset = glm::vec3(0.0f, -1.4f, 1.8f);
	BulletOffset = glm::vec3(0.4f, 0.9f, 0.0f);
	CurrentOffset = Offset;
	FlashOffset = glm::vec3(.05f, 1.1f, 4.3f);
	RayOffset = glm::vec3(.05f, 0.8f, 4.3f);
	aimvector = AimOffset - Offset;
	model.LoadModel("Models/Kalashnikov.obj");

	model.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(.1f, .1f, .1f)), true);
	model.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(90)),true);
}

void Weapon::CreateAwp()
{
	extern Mix_Chunk* sound_awpshot;
	extern Mix_Chunk* sound_awpreload;

	weaponID = 1;
	firingsoundchunk = sound_awpshot;
	reloadsound = sound_awpreload;
	automatic = false;

	maxammo = 8;
	currentammo = 8;
	reloadcounter = 0;
	reloadtime = 255;
	fireratecounter = 0;
	firerate = 100000;
	Offset = glm::vec3(0.8f, -1.8f, 2.8f); //with model
	AimOffset = glm::vec3(0.03f, -2.15f, 1.5f);
	BulletOffset = glm::vec3(0.4f, 1.4f, 0.3f);
	CurrentOffset = Offset;
	FlashOffset = glm::vec3(.05f, 1.7f, 9.3f);
	RayOffset = glm::vec3(.03f, 2.3f, 4.3f);
	aimvector = AimOffset - Offset;
	model.LoadModel("Models/awpdlore.obj");

	model.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(180)));
	model.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 0.0f, 1.0f), Utils::ToRadians(0)));
}

void Weapon::CreatePistol()
{
	extern Mix_Chunk* sound_pistolshot;
	extern Mix_Chunk* sound_pistolreload;
	
	weaponID = 2;

	firingsoundchunk = sound_pistolshot;
	reloadsound = sound_pistolreload;
	automatic = false;

	maxammo = 10;
	currentammo = 10;
	reloadcounter = 0;
	reloadtime = 130;
	fireratecounter = 0;
	firerate = 10000;
	Offset = glm::vec3(1.2f, -1.8f, 3.9f); //with model
	AimOffset = glm::vec3(0.0f, -1.8f, 3.9f);
	BulletOffset = glm::vec3(0.3f, 1.5f, 0.0f);
	CurrentOffset = Offset;
	FlashOffset = glm::vec3(.06f, 1.6f, 2.3f);
	RayOffset = glm::vec3(.06f, 1.6f, 2.3f);
	aimvector = AimOffset - Offset;

	model.LoadModel("Models/M1911.obj");

	model.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(.2f, .2f, .2f)),true);
}

void Weapon::SpawnBullet()
{
	extern Model Bullet;
	extern Model PistolBullet;

	Bullets.reserve(100);
	if (Bullets.size() < Bullets.capacity())
	{
		if (weaponID == 0 || weaponID == 1)
			Bullets.emplace_back(Bullet, glm::mat4(1.0f));
		else if (weaponID == 2)
			Bullets.emplace_back(PistolBullet, glm::mat4(1.0f));
	}
}

void Weapon::UpdateBullet(int i)
{
	float bulletgravity = .10f;
	float bulletspeed = .14f;
	int rotationdelay = 1;

	Bullets[i].bulletmodels.meshList[0]->velocity += glm::vec3(0, -1.0f, 0) * bulletgravity;
	Bullets[i].bulletmodels.meshList[0]->physicscenter += Bullets[i].bulletmodels.meshList[0]->velocity * bulletspeed;

	Bullets[i].bulletlifetimecounter++;
	if(Bullets[i].bulletlifetimecounter > rotationdelay)
		Bullets[i].bulletangle += Bullets[i].randomincrement;

	if (Bullets[i].bulletlifetimecounter >= Bullets[i].bulletlifetime)
	{
		Bullets[i].bulletmodels.ClearMeshes();
		Bullets.erase(Bullets.begin() + i);
	}
}

Weapon::~Weapon()
{
	Mix_FreeChunk(reloadsound);
	reloadsound = nullptr;
	Mix_FreeChunk(firingsoundchunk);
	firingsoundchunk = nullptr;
	Mix_FreeMusic(firingsound);
	firingsound = nullptr;
}
