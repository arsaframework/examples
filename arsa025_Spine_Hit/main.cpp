#include <arsa.h>

irr::arsa::CARSAJson* json = 0;
int id = -1;

void init()
{
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,960,1));
	json->setAnimation("walk");

	g_psd->load("hit.psd");
	g_psd->setDrawOrder(1);
}

void update()
{
	if (g_psd->hitTouch(irr::EKS_RELEASED) == "reset")
	{
		for (irr::u32 i = 0; i < json->getSlotCount(); ++i)
			json->setSlotManual(i, false);
	}

	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
		id = -1;

	if (id == -1)
		id = json->hitTouch(irr::EKS_DOWN);

	if (id != -1)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		json->setSlotManual(id, true);
		json->setSlotPos(id, irr::core::vector3df(x, y, 1));
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
