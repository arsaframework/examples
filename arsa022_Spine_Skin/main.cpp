#include <arsa.h>

irr::arsa::CARSAJson* json = 0;

void init()
{
	json = arsa_AddJson();
	json->load("data/goblins-mesh.json");
	json->setPosition(irr::core::vector3df(320,900,1));
	json->setAnimation("walk");
	json->setSkin("goblin");
	json->setScale(irr::core::vector3df(2));

	g_psd->load("skin.psd");
}

void update()
{
	if (g_psd->hitTouchLayer("goblin", irr::EKS_RELEASED))
		json->setSkin("goblin");
	if (g_psd->hitTouchLayer("goblingirl", irr::EKS_RELEASED))
		json->setSkin("goblingirl");
}

int main( int argc, char* args[] )
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
