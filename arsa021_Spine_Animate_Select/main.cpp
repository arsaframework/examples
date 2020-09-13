#include <arsa.h>

irr::arsa::CARSAJson* json = 0;

void init()
{
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,900,1));
	json->setAnimation("idle");
	json->setScale(irr::core::vector3df(0.8f));
	json->setDrawOrder(1);

	g_psd->load("animate.psd");
	
}

void update()
{
	if (g_psd->hitTouchLayer("idle", irr::EKS_RELEASED))
	{
		if (!json->isPlaying("idle"))
		{
			json->setAnimation("idle");
			json->setAnimation("", true, 1);
		}
	}
	if (g_psd->hitTouchLayer("walk", irr::EKS_RELEASED))
	{
		if (!json->isPlaying("walk") || json->isPlaying("shoot", 1))
		{
			json->setAnimation("walk");
			json->setAnimation("", true, 1);
		}
	}
	if (g_psd->hitTouchLayer("run", irr::EKS_RELEASED))
	{
		if (!json->isPlaying("run") || json->isPlaying("shoot", 1))
		{
			json->setAnimation("run");
			json->setAnimation("", true, 1);
		}
	}
	if (g_psd->hitTouchLayer("jump", irr::EKS_RELEASED))
	{
		if(!json->isPlaying("jump"))
			json->setAnimation("jump");
	}
	if (g_psd->hitTouchLayer("walk+fire", irr::EKS_RELEASED))
	{
		if (!json->isPlaying("walk")|| !json->isPlaying("shoot",1))
		{
			json->setAnimation("walk");
			json->setAnimation("shoot", true, 1);
		}
	}
	if (g_psd->hitTouchLayer("run+fire", irr::EKS_RELEASED))
	{
		if (!json->isPlaying("run") || !json->isPlaying("shoot", 1))
		{
			json->setAnimation("run");
			json->setAnimation("shoot", true, 1);
			json->setSpeed(10, 1);
		}
	}
}

int main( int argc, char* args[] )
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
