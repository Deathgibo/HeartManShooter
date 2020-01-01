#include "Player.h"

Player::Player()
{
	model_body = nullptr;
	weapon = nullptr;
	position = glm::vec3(0, 40, 0);
	velocity = glm::vec3(0, 0, 0);
	jumpvector = glm::vec3(0, 0, 0);

	feetonground = true;
	aimcounter = 0;
	reloading = false;
	shooting = false;
	canshootagain = true;
	canaimagain = true;
	kickback = false;
	kickbackpercent = 0.0f;
	kickbackdir = true;
	flashon = false;
	shootray = false;

}

Player::Player(Model* body, Weapon* gun)
{
	model_body = body;
	weapon = gun;
	position = glm::vec3(0, 40, 0);
	velocity = glm::vec3(0, 0, 0);
	jumpvector = glm::vec3(0, 0, 0);

	feetonground = true;

	//hitbox(position, 2.9, 5.5, 2.7, true);
	//hitbox.Initialize(position, 2.9, 5.5, 2.7, true);
	aimcounter = 0;
	reloading = false;
	shooting = false;
	canshootagain = true;
	canaimagain = true;
	kickback = false;
	kickbackpercent = 0.0f;
	kickbackdir = true;
	flashon = false;
	shootray = false;
}

void Player::Initialize(Model* body, Weapon* gun)
{
	model_body = body;
	weapon = gun;
	position = glm::vec3(0, 40, 0);
	velocity = glm::vec3(0, 0, 0);

	//nanosuit (0,-6.1,0.4) , 2.1, 7.4, 1.6
	//mario (0, -2.2, 2.6) 2.9, 5.5, 2.7
	hitbox.Initialize(position, 2.9, 5.5, 2.7, true);
	aimcounter = 0;
	reloading = false;
	shooting = false;
	canshootagain = true;
	canaimagain = true;
	kickback = false;
	kickbackpercent = 0.0f;
	kickbackdir = true;
	flashon = false;
	shootray = false;
}

void Player::PlayerUpdate(Singlekeys* singlekeys)
{
	extern Mix_Music* sound_akfiring;
	extern Mix_Chunk* sound_akreload;
	extern Mix_Chunk* sound_aklastbullet;
	extern Camera camera;
	extern glm::mat4 pMat;
	extern bool rightclick;
	extern bool leftclick;
	extern int width;
	extern int height;
	extern GLFWwindow* window;

	/*Aiming*/
	if (rightclick && !reloading && canaimagain)
	{
		if (aimcounter < 104)
		{
			weapon->CurrentOffset += weapon->aimvector*.0769f;
			aimcounter += 8;
		}
	}
	else
	{
		if (aimcounter >= 0)
		{
			weapon->CurrentOffset -= weapon->aimvector*.0769f;
			aimcounter -= 8;
		}
	}

	if (aimcounter == 104 && weapon->weaponID == 1)
	{
		glfwGetFramebufferSize(window, &width, &height);
		float aspect = (float)width / (float)height;
		pMat = glm::perspective(.2f, aspect, 0.1f, 2000.0f);//1.0472f
		weaponscoped = true;
	}
	else
	{
		glfwGetFramebufferSize(window, &width, &height);
		float aspect = (float)width / (float)height;
		pMat = glm::perspective(1.0472f, aspect, 0.1f, 2000.0f);//1.0472f
		weaponscoped = false;
	}

	/*Shooting & Reloading*/
	if (leftclick == false)
	{
		canshootagain = true;
	}
	if (rightclick == false)
	{
		canaimagain = true;
	}
	if (rightclick == true && weapon->currentammo > 0 && canaimagain)
	{
		reloading = false;
	}
	if (leftclick && weapon->currentammo > 0 && canshootagain)
	{
		reloading = false;
		shooting = true;
	}
	else
	{
		shooting = false;
	}
	if ((singlekeys[GLFW_KEY_R].key || weapon->currentammo <= 0) && (weapon->currentammo != weapon->maxammo) && reloading == false)
	{
		Mix_PlayChannel(0, weapon->reloadsound, 0);
		reloading = true;
		shooting = false;
		canshootagain = false;
		canaimagain = false;

		weapon->reloadcounter = 0;
	}
	if (reloading)
	{
		weapon->reloadcounter++;
		if (weapon->reloadcounter >= weapon->reloadtime)
		{
			weapon->currentammo = weapon->maxammo;
			reloading = false;
			canaimagain = true;
			canshootagain = true;
		}
		//cout << "Reload counter " << weapon->reloadcounter << endl;
	}
	else
	{
		Mix_HaltChannel(0);
	}
	if (shooting)
	{
		if (!Mix_PlayingMusic() && weapon->automatic)
		{
			Mix_PlayMusic(weapon->firingsound, 1);
		}

		if (weapon->fireratecounter % weapon->firerate == 0)
		{
			if (!weapon->automatic)
			{
				Mix_PlayChannel(1, weapon->firingsoundchunk, 0);
			}
			weapon->SpawnBullet();
			weapon->currentammo--;

			camera.pitch -= 1.f;
			kickback = true;
			flashon = true;
			flashrotation = rand() % 360;
			shootray = true;
		}
		weapon->fireratecounter++;
		//cout << "Current Ammo: " << weapon->currentammo << endl;
	}
	else
	{
		weapon->fireratecounter = 0;//everytime you dont shoot reset under assumption you can't click faster than firerate
		if (Mix_PlayingMusic() && weapon->automatic)
		{
			Mix_HaltMusic();
			Mix_PlayChannel(1, sound_aklastbullet, 0);
		}
	}

	/*Bullets*/
	for (int i = 0; i < weapon->Bullets.size(); i++)
	{
		weapon->UpdateBullet(i);
	}

	/*Bullet Holes*/
	for (int i = 0; i < bulletholes.size(); i++)
	{
		bulletholes[i]->timer++;
		if (bulletholes[i]->timer > 800)
		{
			delete bulletholes[i]; bulletholes[i] = nullptr;
			bulletholes.erase(bulletholes.begin() + i);
		}
	}

	/*Kickback*/
	if (kickback)
	{
		kickbackroutine();
	}
}

void Player::Shootray()
{
	extern glm::vec3 MarioOffset;
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;
	extern bool rightclick;
	extern vector<Triangle> WorldTriangles;
	extern Camera camera;

	Ray r;
	glm::vec4 rayposition = glm::vec4(weapon->RayOffset.x, weapon->RayOffset.y, weapon->RayOffset.z, 1.0f);
	//glm::vec4 rayposition = glm::vec4(camera.position.x, camera.position.y, camera.position.z, 1.0f);
	//glm::vec4 rayposition(0, 0, 0, 1);

	mMat = glm::mat4(1.0f);
	mMat = glm::translate(mMat, (-camera.lookdirection * kickbackpercent));//move weapon kickback amount
	mMat = glm::translate(mMat, camera.rightdirection*weapon->CurrentOffset.x + camera.updirection*weapon->CurrentOffset.y + camera.lookdirection*weapon->CurrentOffset.z);//move weapon offset;
	mMat = glm::translate(mMat, glm::vec3(0, MarioOffset.y, 0) + MarioOffset.x*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z)));//move camera model offset
	mMat = glm::translate(mMat, position);
	mMat = mMat * Utils::GetRodriguezMatrix(camera.rightdirection, glm::radians(-camera.pitch + 90.0f + 1.0f));
	mMat = mMat * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-camera.yaw - 90.0f));

	//mvMat = vMat * mMat;

	rayposition = mMat * rayposition;
	glm::vec3 rayposition3 = glm::vec3(rayposition.x, rayposition.y, rayposition.z);

	glm::vec3 raydirection = camera.lookdirection;
	raydirection = glm::normalize(raydirection);

	r.setray(rayposition3, raydirection);

	float tvalue = -1;
	float lowestt = 10000;
	Triangle thetri;
	for (int i = 0; i < WorldTriangles.size(); i++)
	{
		tvalue = Utils::RayTriCollision(r, WorldTriangles[i]);
		if (tvalue != -1 && tvalue < lowestt)
		{
			//cout << "collision!";
			lowestt = tvalue;
			thetri = WorldTriangles[i];
		}
	}

	if (lowestt != 10000)//if it hit something
	{
		/*calculate center*/
		lowestt -= .1f;//.1f
		glm::vec3 thecenter = rayposition3 + (lowestt * raydirection);

		/*calculate basis vectors*/
		glm::vec3 upvector(0.0f, 1.0f, 0.0f);
		glm::vec3 rightvector(1.0f, 0.0f, 0.0f);
		glm::vec3 normalvector = Utils::CalculateNormal(thetri.one, thetri.two, thetri.three);
		normalvector = glm::normalize(normalvector);

		float ntheta = 1.57f;
		float nphi = 1.57f;
		float rtheta = 0.0f;
		float rphi = 1.57f;

		float phi, theta;
		if (normalvector.y > 1)
			normalvector.y = 1;
		if (normalvector.y < -1)
			normalvector.y = -1;
		assert(normalvector.y <= 1 && normalvector.y >= -1, "noramalvector.y needs to fit acos domain 1");
		
		phi = acos(normalvector.y);
		
		if (phi < .01)
			phi = .01;

		float calc = normalvector.x / sin(phi);
		if (calc > 1)
			calc = 1;
		if (calc < -1)
			calc = -1;
		assert(calc <= 1 && calc >= -1, "noramalvector.y needs to fit acos domain 2");
		
		theta = acos(calc);
		
		if (normalvector.z < 0)
		{
			theta = (3.14 * 2) - theta;
		}

		float thetadiff = theta - ntheta ;

		rightvector.x = sin(rphi)*cos(rtheta + thetadiff);
		rightvector.y = cos(rphi);
		rightvector.z = sin(rphi)*sin(rtheta + thetadiff);
		rightvector = glm::normalize(rightvector);

		upvector = glm::cross(rightvector, normalvector);
		upvector = glm::normalize(upvector);
		
		BulletHole* tmp = new BulletHole(normalvector, rightvector, upvector, thecenter, 1, 1);
		bulletholes.push_back(tmp);
	}
}

void Player::kickbackroutine()
{
	float kickbackspeed = .5f;

	if (kickbackdir)
	{
		kickbackpercent += kickbackspeed;
	}
	else
	{
		kickbackpercent -= kickbackspeed;
	}
	
	if (kickbackpercent > 2)
	{
		kickbackdir = false;
	}
	if (kickbackpercent <= 0)
	{
		kickbackdir = true;
		kickback = false;
	}
}

void Player::RenderPlayer(Shader* theshader, bool shadowpass)
{
	extern Shader shader2d;
	extern glm::vec3 MarioOffset;
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;
	extern Camera camera;
	extern bool rightclick;
	extern Texture flashtex;
	extern Texture bullethole;
	extern Texture sniperscope;
	extern Rectangle flashrect;
	extern Square scoperect;
	extern vector<Triangle> WorldTriangles;
	extern Material mat_nospecular;
	extern Material mat_specularsmall;
	extern Material mat_gold;
	extern Material mat_jade;
	extern ShadowMap shadow_diffuse1;

	if (shootray)
	{
		Shootray();
		shootray = false;
	}

	/*Render body*/
	mMat = glm::mat4(1.0f);
	mMat = glm::translate(mMat, (position));
	mMat = mMat * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(-camera.yaw));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(theshader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(theshader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	model_body->RenderModel();

	/*Render Bulletholes*/
	if (!shadowpass)
	{
		for (int i = 0; i < bulletholes.size(); i++)
		{
			glUniform1i(theshader->isflash, 1);

			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, bulletholes[i]->rect.center);//move weapon offset;
			mvMat = vMat * mMat;
			glUniformMatrix4fv(theshader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
			glUniformMatrix4fv(theshader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

			bullethole.UseTexture();
			bulletholes[i]->rect.RenderMesh();

			glUniform1i(theshader->isflash, 0);
		}
	}

	if (weaponscoped)
	{
		glUseProgram(shader2d.program);
		mMat = glm::mat4(1.0f);
		mvMat = mMat;
		glUniformMatrix4fv(shader2d.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniform1i(shader2d.renderdepthmap, 0);
		sniperscope.UseTexture();
		scoperect.RenderMesh();

		glUseProgram(theshader->program);
	}
	else
	{
		/*Render Flash*/
		if (flashon)
		{
			glUniform1i(theshader->isflash, 1);

			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, camera.rightdirection*weapon->FlashOffset.x + camera.updirection*weapon->FlashOffset.y + camera.lookdirection*weapon->FlashOffset.z);//move flash offset
			mMat = glm::translate(mMat, (-camera.lookdirection * kickbackpercent));//move weapon kickback amount
			mMat = glm::translate(mMat, camera.rightdirection*weapon->CurrentOffset.x + camera.updirection*weapon->CurrentOffset.y + camera.lookdirection*weapon->CurrentOffset.z);//move weapon offset;
			mMat = glm::translate(mMat, glm::vec3(0, MarioOffset.y, 0) + MarioOffset.x*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z)));//move camera model offset
			mMat = glm::translate(mMat, position);
			mMat = mMat * Utils::GetRodriguezMatrix(camera.lookdirection, Utils::ToRadians(flashrotation));
			mMat = mMat * Utils::GetRodriguezMatrix(camera.rightdirection, glm::radians(-camera.pitch + 90.0f));
			mMat = mMat * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-camera.yaw - 90.0f));
			mvMat = vMat * mMat;
			glUniformMatrix4fv(theshader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
			glUniformMatrix4fv(theshader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

			flashtex.UseTexture();
			flashrect.RenderMesh();

			glUniform1i(theshader->isflash, 0);
			flashoncounter++;
			if (flashoncounter >= 10)
			{
				flashon = false;
				flashoncounter = 0;
			}
		}


		/*Render Weapon*/
		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, (-camera.lookdirection * kickbackpercent));//move weapon kickback amount
		mMat = glm::translate(mMat, camera.rightdirection*weapon->CurrentOffset.x + camera.updirection*weapon->CurrentOffset.y + camera.lookdirection*weapon->CurrentOffset.z);//move weapon offset;
		mMat = glm::translate(mMat, glm::vec3(0, MarioOffset.y, 0) + MarioOffset.x*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z)));//move camera model offset
		mMat = glm::translate(mMat, position);
		mMat = mMat * Utils::GetRodriguezMatrix(camera.rightdirection, glm::radians(-camera.pitch + 90.0f));
		mMat = mMat * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-camera.yaw - 90.0f));

		mvMat = vMat * mMat;
		glUniformMatrix4fv(theshader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(theshader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		mat_specularsmall.UseMaterial(theshader);
		//mat_gold.UseMaterial(theshader);
		weapon->RenderWeapon();
		mat_nospecular.UseMaterial(theshader);
	}

	/*Render Bullets*/
	if (!shadowpass)
	{
		for (int i = 0; i < weapon->Bullets.size(); i++)
		{
			if (weapon->Bullets[i].firsttime == true)//weapon->Bullets[i].firsttime == true
			{
				weapon->Bullets[i].firsttime = false;
				//*small small Random velocity
				//*and random rotation
				weapon->Bullets[i].randomincrement = rand() % 21;
				if (weapon->Bullets[i].bulletmodels.meshList.empty())
				{
					cout << "bullet doenst have model" << endl;
				}
				else
				{
					weapon->Bullets[i].bulletmodels.meshList[0]->velocity = camera.rightdirection + (camera.lookdirection * (((rand() % 200) / 100.0f) - 1.0f));
				}

				/*switch this code outside brackets if you want bullets to follow or not*/
				glm::vec3 bulletoffsetcamera = (weapon->BulletOffset.x * camera.rightdirection) + (weapon->BulletOffset.y * camera.updirection) + (weapon->BulletOffset.z * camera.lookdirection);

				mMat = glm::mat4(1.0f);
				mMat = glm::translate(mMat, bulletoffsetcamera);//bullet offset
				mMat = glm::translate(mMat, (-camera.lookdirection * kickbackpercent));//move weapon kickback amount
				mMat = glm::translate(mMat, camera.rightdirection*weapon->CurrentOffset.x + camera.updirection*weapon->CurrentOffset.y + camera.lookdirection*weapon->CurrentOffset.z);//move weapon offset;
				mMat = glm::translate(mMat, glm::vec3(0, MarioOffset.y, 0) + MarioOffset.x*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z)));//move camera model offset
				mMat = glm::translate(mMat, position);
				mMat = mMat * Utils::GetRodriguezMatrix(camera.updirection, Utils::ToRadians(weapon->Bullets[i].bulletangle));
				mMat = mMat * Utils::GetRodriguezMatrix(camera.rightdirection, glm::radians(-camera.pitch + 90.0f));
				mMat = mMat * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-camera.yaw - 90.0f));

				weapon->Bullets[i].bulletmatrix = mMat;
			}

			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, glm::vec3(weapon->Bullets[i].bulletmodels.meshList[0]->physicscenter.x, weapon->Bullets[i].bulletmodels.meshList[0]->physicscenter.y,
				weapon->Bullets[i].bulletmodels.meshList[0]->physicscenter.z));
			mMat = mMat * weapon->Bullets[i].bulletmatrix;
			mMat = mMat * Utils::GetRodriguezMatrix(camera.updirection, Utils::ToRadians(weapon->Bullets[i].bulletangle));
			mvMat = vMat * mMat;
			glUniformMatrix4fv(theshader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
			glUniformMatrix4fv(theshader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
			weapon->Bullets[i].bulletmodels.RenderModel();
		}
	}


}

void Player::SwapWeapon()
{
	//delete all bullets
	//set currentoffset to offset
	//cancel reload

	if (Mix_PlayingMusic() && weapon->automatic)
	{
		Mix_HaltMusic();
	}

	weapon->CurrentOffset = weapon->Offset;
	
	aimcounter = 0;
	reloading = false;
	shooting = false;
	canshootagain = true;
	canaimagain = true;
	kickback = false;
	kickbackpercent = 0.0f;
	kickbackdir = true;
	flashon = false;


}
void Player::HandleInput(bool * keys)
{
	extern Singlekeys singlekeys[1024];
	extern Camera camera;
	extern bool noclip;

	velocity = glm::vec3(0, 0, 0);
	
	if (noclip)
	{
		if (keys[GLFW_KEY_W])
		{
			velocity += camera.lookdirection;
		}
		if (keys[GLFW_KEY_S])
		{
			velocity += -camera.lookdirection;
		}
		if (keys[GLFW_KEY_A])
		{
			velocity += -camera.rightdirection;
		}
		if (keys[GLFW_KEY_D])
		{
			velocity += camera.rightdirection;
		}
		if (keys[GLFW_KEY_LEFT_CONTROL])
		{
			velocity -= camera.updirection;
		}
		if (keys[GLFW_KEY_SPACE])
		{
			velocity += camera.updirection;
		}
	}
	else
	{
		glm::vec3 xvector = glm::normalize(camera.rightdirection);
		glm::vec3 zvector = glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z));

		if (keys[GLFW_KEY_W])
		{
			velocity += zvector;
		}
		if (keys[GLFW_KEY_S])
		{
			velocity += -zvector;
		}
		if (keys[GLFW_KEY_A])
		{
			velocity += -xvector;
		}
		if (keys[GLFW_KEY_D])
		{
			velocity += xvector;
		}
		if (keys[GLFW_KEY_SPACE] && feetonground)
		{
			jumpvector = glm::vec3(0, 2.0f, 0);
			feetonground = false;
		}
	}

	glm::normalize(velocity);
	MovePlayer();
}
//*one note is that feetonground switching between 0 and 1 so its not frame perfect when your actually on the ground
void Player::MovePlayer()//do this in update function
{
	extern bool noclip;
	
	movespeed = 1.0f;
	float gravityincrement = .050f;
	glm::vec3 gravity(0, -1.0f, 0);

	/*gravity*/
	/*if(!noclip)
		velocity += gravity;*/

	if (!feetonground && !noclip)
	{
		jumpvector += gravity * gravityincrement;
	}
	else
	{
		jumpvector = glm::vec3(0, 0, 0);
	}
	
	velocity += jumpvector;

	/*velocity*/
	position += velocity * movespeed;
	hitboxOffset = glm::vec3(0, -0.2f, 0.0f);
	hitbox.center = position + hitboxOffset;
	HandlePhysics();
}

void Player::HandlePhysics()
{
	extern vector<HitBox> staticHitBoxes;
	extern vector<HitPlane> staticHitPlanes;
	
	glm::vec3 fl = glm::vec3(hitbox.center.x - hitbox.width, hitbox.center.y - hitbox.height, hitbox.center.z + hitbox.thedepth);
	glm::vec3 fr = glm::vec3(hitbox.center.x + hitbox.width, hitbox.center.y - hitbox.height, hitbox.center.z + hitbox.thedepth);
	glm::vec3 bl = glm::vec3(hitbox.center.x - hitbox.width, hitbox.center.y - hitbox.height, hitbox.center.z - hitbox.thedepth);
	glm::vec3 br = glm::vec3(hitbox.center.x + hitbox.width, hitbox.center.y - hitbox.height, hitbox.center.z - hitbox.thedepth);
	HitPlane footplane(fl, fr, bl, br, false, glm::vec3(0, 0, 0), 0, false);

	glm::vec3 originaljumpvector = jumpvector;

	feetonground = false;
	float yspeed = .01f;
	for (int i = 0; i < staticHitPlanes.size(); i++)
	{
		if(Utils::planeplanecollision(footplane,staticHitPlanes[i]))
		{
			bool stillhitting = true;
			while(stillhitting)
			{
				position.y+= yspeed;
				hitbox.center.y+= yspeed;
				footplane.Translate(0, yspeed, 0);
				if(!Utils::planeplanecollision(footplane,staticHitPlanes[i]))
				{
					stillhitting = false;
				}
			}
			feetonground = true;
		}
	}
	for (int i = 0; i < staticHitBoxes.size(); i++)
	{
		if (Utils::boxboxcollision(hitbox, staticHitBoxes[i]))//check x
		{
			position.x -= (velocity.x * movespeed);
			hitbox.center = position + hitboxOffset;

			if (Utils::boxboxcollision(hitbox, staticHitBoxes[i]))//move x back check y
			{
				position.x += velocity.x * movespeed;
				position.y -= velocity.y * movespeed;

				if(velocity.y <= 0)
					feetonground = true;

				jumpvector = glm::vec3(0, 0, 0);
				hitbox.center = position + hitboxOffset;
				if (Utils::boxboxcollision(hitbox, staticHitBoxes[i]))//move y back check z
				{
					position.y += velocity.y * movespeed;
					position.z -= velocity.z * movespeed;

					feetonground = false;
					jumpvector = originaljumpvector;
					hitbox.center = position + hitboxOffset;
				}
			}
		}
	}
	for (int j = 0; j < staticHitPlanes.size(); j++)
	{
		for (int i = 0; i < staticHitPlanes[j].hitboxes.size(); i++)
		{
			if (Utils::boxboxcollision(hitbox, staticHitPlanes[j].hitboxes[i]))//check x
			{
				position.x -= (velocity.x * movespeed);
				hitbox.center = position + hitboxOffset;

				if (Utils::boxboxcollision(hitbox, staticHitPlanes[j].hitboxes[i]))//move x back check y
				{
					position.x += velocity.x * movespeed;
					position.z -= velocity.z * movespeed;
					feetonground = false;
					hitbox.center = position + hitboxOffset;
					if (Utils::boxboxcollision(hitbox, staticHitPlanes[j].hitboxes[i]))//move y back check z
					{
						position.z += velocity.z * movespeed;
						position.y -= velocity.y * movespeed;
						if (velocity.y <= 0)
							feetonground = true;
						jumpvector = glm::vec3(0, 0, 0);
						hitbox.center = position + hitboxOffset;
					}
				}
			}
		}
	}
}

Player::~Player()
{
}
