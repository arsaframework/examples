#include <arsa.h>

irr::arsa::CARSAJson* json = 0;
float r;

irr::f32 Missile_Time = 0;

// missile structure
struct SMissile
{
	irr::core::stringc Name;
	irr::core::vector3df pos, vel;
};
irr::core::array<SMissile> mlist;

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
		mlist[i].pos += mlist[i].vel;

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

		// out of screen
		//if (mlist[i].pos.Y <= 0)
		if(g_psd->isPsdOutOfScreen(mid))
		{
			// kill missile
			g_psd->removePsd(mid);

			// kill data in manager
			mlist.erase(i);

			// after kill data in manager MUST exit loop always
			// because list in manager changed
			return;
		}
	}
}

void init()
{
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,680,1));
	json->setScale(irr::core::vector3df(0.5f));
	json->setAnimation("run");

	g_psd->load("crosshair.psd");
}

void update()
{
	// get bone pos
	irr::core::stringc bone("rear_upper_arm");
	irr::core::vector3df pos(json->getBonePos(bone));
	irr::core::vector3df tar;

	if ( g_input->getTouchState(0) == irr::EKS_DOWN)
	{
		// touch pos
		int tx = g_input->getTouchX(0);
		int ty = g_input->getTouchY(0);
		tar = irr::core::vector3df(tx, ty, 1);

		// set crosshair to touch pos
		g_psd->setPsdPos(g_psd->getPsdIdByPrefix("crosshair"), tx, ty);

		// play shoot animation
		if (!json->isPlaying("shoot", 1))
		{
			json->setAnimation("shoot", true, 1);
			json->setSpeed(20, 1);
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
			irr::core::vector3df mvel = (tar - pos).normalize() * 8;
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

	UpdateMis();
	RemoveMis();
}

int main( int argc, char* args[] )
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
