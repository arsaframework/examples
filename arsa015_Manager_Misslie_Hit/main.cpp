#include <arsa.h>

int jet = -1;
int m1 = -1;
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
		if (mlist[i].pos.Y <= 0)
		{
			// kill missile
			g_psd->removePsd(mid);

			// kill data in manager
			mlist.erase(i);

			// after kill data in manager MUST exit loop always
			// because list in manager changed
			return;
		}

		// hit with box
		int hit_id = g_psd->hitId(mid);
		if (hit_id != -1)
		{
			// reject hit with jet
			if (hit_id == jet)
				continue;

			// reject hit player missile
			irr::core::stringc hit_name = g_psd->getPsdName(hit_id);
			if (hit_name.find("player_missile") != -1)
				continue;

			g_psd->setPsdVisible(hit_id, false);
			g_psd->removePsd(mid);
			mlist.erase(i);
			return;
		}
	}
}

void init()
{
	g_psd->load("missile.psd"); // cache psd image 
	jet = g_psd->getPsdIdByPrefix("jet"); // get jet id
	m1 = g_psd->getPsdId("m1"); // get missile id
}

void update()
{
	// move jet
	if (jet != -1)
	{
		irr::f32 x, y;
		irr::f32 speed = 500 * g_psd->getDeltaTime();
		g_psd->getPsdPos(jet, x, y);
		if (g_input->isTouchLeft(0))
			x -= speed;
		if (g_input->isTouchRight(0))
			x += speed;
		if (g_input->isTouchUp(0))
			y -= speed;
		if (g_input->isTouchDown(0))
			y += speed;
		g_psd->setPsdPos(jet, x, y);
	}

	// fire missile
	if (g_input->getTouchState(0) == irr::EKS_DOWN)
	{
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

		// fire
		if (fire)
		{
			// get current jet pos
			irr::core::vector3df pos;
			g_psd->getPsdPos(jet, pos.X, pos.Y);

			// add missile to manager
			// m1 is missile image id from psd
			// pos is jet's current position
			// ::core::vector3df(0, -20, 0) is velocity, fast=more value
			AddMis(m1, pos, irr::core::vector3df(0, -20, 0));
		}
	}
	else
		Missile_Time = 0;

	// manager process, update and remove
	UpdateMis();
	RemoveMis();
}

int main(int argc, char* args[])
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart();

	return 0;
}
