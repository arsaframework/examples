#include <arsa.h>

irr::arsa::CARSAJson* json = 0;
float r;
void init()
{
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,960,1));
	json->setAnimation("run");

	g_psd->load("crosshair.psd");
}

void update()
{

	irr::core::vector3df tar;
	if ( g_input->getTouchState(0) == irr::EKS_DOWN)
	{
		int tx = g_input->getTouchX(0);
		int ty = g_input->getTouchY(0);
		tar = irr::core::vector3df(tx, ty, 1);

		g_psd->setPsdPos(g_psd->getPsdIdByPrefix("crosshair"), tx, ty);

		if (!json->isPlaying("shoot", 1))
		{
			json->setAnimation("shoot", true, 1);
			json->setSpeed(20, 1);
		}
	}
	else
	{
		irr::f32 tx, ty;
		g_psd->getPsdPos(g_psd->getPsdIdByPrefix("crosshair"), tx, ty);
		tar = irr::core::vector3df(tx, ty, 1);

		if (json->isPlaying("shoot", 1))
			json->setAnimation("", true, 1);
	}

	irr::core::stringc bone("rear_upper_arm");
	irr::core::vector3df pos(json->getBonePos(bone));	
	json->setBoneRot(bone, arsa_GetZRot(pos, tar)-10);
}

int main( int argc, char* args[] )
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
