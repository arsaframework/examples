#include <arsa.h>

irr::arsa::CARSAJson* json = 0;
irr::arsa::CARSAJson* goblin = 0;
float r;
irr::u32 killed = 0;

irr::f32 Missile_Time = 0;
irr::u32 Enemy_Time = 0;

// missile structure
struct SMissile
{
	irr::core::stringc Name;
	irr::core::vector3df pos, vel;
};
irr::core::array<SMissile> mlist;

struct SEnemy
{
	irr::f32 hp, hp_max;
	irr::arsa::CARSAJson* Json;
	irr::core::vector3df pos, vel;
};
irr::core::array<SEnemy> enemy_list;

void AddEnemy(irr::arsa::CARSAJson* js, const irr::core::vector3df& pos, const irr::core::vector3df& vel)
{
	SEnemy enemy;
	enemy.Json = js->clone( );
	enemy.Json->setSkin((g_rand->rand()%2)?"goblin": "goblingirl");
	enemy.Json->setAnimation("walk");
	enemy.Json->setHFlip(true);
	enemy.Json->setVisible(true);
	enemy.Json->setDrawOrder(1);
	irr::f32 scale = arsa_clamp_float(g_rand->frand(), 0.1f, 1.0f);	
	enemy.Json->setScale(irr::core::vector3df(scale *2.5f));
	enemy.Json->setPosition(pos);
	enemy.pos = pos;
	enemy.vel = vel;
	enemy.hp = scale*2;
	enemy.hp_max = enemy.hp;

	enemy_list.push_back(enemy);
}

void UpdateEnemy(void)
{
	for (irr::u32 i = 0; i < enemy_list.size(); ++i)
	{
		SEnemy& enemy = enemy_list[i];

		// pos translate
		enemy.pos += enemy.vel * g_psd->getDeltaTime();

		// set a new pos to json
		enemy.Json->setPosition( enemy.pos );
	}
}

void RemoveEnemy()
{
	for (irr::u32 i = 0; i < enemy_list.size(); ++i)
	{
		SEnemy& enemy = enemy_list[i];

		// out of screen
		if (enemy.Json->isOutOfScreen())
		{
			enemy.Json->remove();

			// kill data in manager
			enemy_list.erase(i);

			// after kill data in manager MUST exit loop always
			// because list in manager changed
			continue;
		}
	}
}

void AddMis(int id, const irr::core::vector3df& pos, const irr::core::vector3df& vel)
{
	SMissile ms;
	// !!!name very important!!!
	// MUST UNIQUENESS
	// use random function for make a difference number
	irr::u32 count = g_rand->rand();
	ms.Name = "player_missile"; // prefix name
	ms.Name += count; // string cat

	// clone image by id
	if (g_psd->clonePsd(id, ms.Name) == -1)
		return;

	// set data
	ms.pos = pos;
	ms.vel = vel;
	int mid = g_psd->getPsdId(ms.Name);

	// visible missile bacause original is hide layer
	g_psd->setPsdVisible(mid, true);
	g_psd->setPsdDrawOrder(mid, 99);

	// add to manager
	mlist.push_back(ms);
}

void UpdateMis(void)
{
	for (irr::u32 i = 0; i < mlist.size(); ++i)
	{
		// get id by missile name
		int mid = g_psd->getPsdId(mlist[i].Name);

		// pos translate
		mlist[i].pos += mlist[i].vel * g_psd->getDeltaTime();

		// set a new pos to psd image
		g_psd->setPsdPos(mid, mlist[i].pos.X, mlist[i].pos.Y);
	}
}

void RemoveMis()
{
	for (irr::u32 i = 0; i < mlist.size(); ++i)
	{
		// get id by missile name
		int mid = g_psd->getPsdId(mlist[i].Name);
		bool remove = false;

		// out of screen
		if (g_psd->isPsdOutOfScreen(mid))
			remove = true;

		// hit with enemy
		if (!remove)
		{
			for (irr::u32 j = 0; j < enemy_list.size(); ++j)
			{
				SEnemy& enemy = enemy_list[j];

				irr::s32 head = enemy.Json->getSlotId("head");
				if (!g_psd->hitBox(enemy.Json->getSlotBBox(head), mid))
					continue;

				enemy.hp -= 0.2f;
				enemy.vel.X += 20;
				irr::f32 rr = ((enemy.hp * 255) / enemy.hp_max);
				enemy.Json->setSlotColor(head, irr::video::SColor(255, 255, rr, rr));
				if (enemy.hp <= 0)
				{
					killed++;
					enemy.Json->remove();
					enemy_list.erase(j);
				}

				remove = true;
				break;
			} // for j
		}

		if(remove)
		{
			// kill missile
			g_psd->removePsd(mid);

			// kill data in manager
			mlist.erase(i);

			// after kill data in manager MUST restart loop always
			// because list in manager changed
			continue;
		}

	} // for i
}

void init()
{
	// load spine boy
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(150,640,1));
	json->setScale(irr::core::vector3df(0.5f));
	json->setAnimation("run");
	//json->setDebugDataVisible(1);
	json->setDrawOrder(2);

	// load goblib original
	goblin = arsa_AddJson();
	goblin->load("data/goblins-mesh.json");
	goblin->setVisible(false);
	//goblin->setDebugDataVisible(1);

	g_psd->load("crosshair.psd");
	g_psd->setPsdDrawOrder(g_psd->getPsdIdByPrefix("crosshair"), 99);
}

void update()
{
	// get bone pos
	irr::core::stringc bone("rear_upper_arm");
	irr::core::vector3df pos(json->getBonePos(bone));
	irr::core::vector3df tar;
	irr::s32 cid = g_psd->getPsdIdByPrefix("crosshair");
	g_psd->getPsdPos(cid, tar.X, tar.Y);

	if ( g_input->getTouchState(0) == irr::EKS_DOWN)
	{
		// touch pos
		int tx = g_input->getTouchX(0);
		int ty = g_input->getTouchY(0);
		tar = irr::core::vector3df(tx, ty, 1);

		// set crosshair to touch pos
		g_psd->setPsdPos(cid, tx, ty);

		// play shoot animation
		if (!json->isPlaying("shoot", 1))
		{
			json->setAnimation("shoot", true, 1);
			json->setSpeed(5, 1);
		}

		bool fire = false;

		// get touch time
		irr::f32 ttime = g_input->getTouchTimeSec(0);

		// first touch
		if (Missile_Time == 0)
			Missile_Time = ttime;

		// check delay
		if (g_input->getTouchTimeSec(0) > Missile_Time)
		{
			Missile_Time = ttime + 0.07f; // next time, plus 0.07 sec
			fire = true;
		}


		if (fire)
		{
			irr::s32 missile = g_psd->getPsdIdByPrefix("missile2");

			// rotate psd by target
			irr::f32 z = arsa_GetZRot(pos, tar);
			g_psd->setPsdRot(missile, irr::core::vector3df(0,0,-z));

			// add missile
			irr::core::vector3df mpos = json->getBonePos("gunTip");
			irr::core::vector3df mvel = (tar - pos).normalize() * 500;
			AddMis(missile, mpos, mvel);
		}
			
	}
	else
	{
		Missile_Time = 0;

		irr::f32 tx, ty;
		g_psd->getPsdPos(g_psd->getPsdIdByPrefix("crosshair"), tx, ty);
		tar = irr::core::vector3df(tx, ty, 1);

		if (json->isPlaying("shoot", 1))
			json->setAnimation("", true, 1);
	}
	json->setBoneRot(bone, arsa_GetZRot(pos, tar)-10);

	irr::u32 timeMs = g_device->getTimer()->getTime();
	if (Enemy_Time == 0) Enemy_Time = timeMs;
	if (timeMs > Enemy_Time)
	{
		Enemy_Time = timeMs + 1000;

		irr::core::vector3df epos, evel;
		epos = irr::core::vector3df(1000,640,1);
		AddEnemy(goblin, epos, irr::core::vector3df(-200,0,1));
	}

	UpdateMis();
	RemoveMis();

	UpdateEnemy();
	RemoveEnemy();
}

void hud()
{
	irr::core::stringw score("Score: ");
	score += killed;
	arsa_DrawFont(arsa_GetFont("TS-Prachachon-NP.ttf", 80), 10, 10, score);
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_SetFuncHud(hud);
	arsa_EasyStart( );

	return 0;
}
