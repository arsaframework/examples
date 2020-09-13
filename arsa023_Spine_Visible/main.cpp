#include <arsa.h>

irr::arsa::CARSAJson* json = 0;
irr::core::array<irr::s32> raptor_ids;

void init()
{
	json = arsa_AddJson();
	json->load("data/raptor.json");
	json->setPosition(irr::core::vector3df(450,640,1));
	json->setAnimation("walk");
	json->setScale(irr::core::vector3df(0.5f));

	json->getSlotIdByPrefixs("raptor_", raptor_ids);

	g_psd->load("slot.psd");
}

void update()
{
	if (g_psd->hitTouchLayer("boy", irr::EKS_RELEASED))
	{
		for (irr::u32 i = 0; i < json->getSlotCount(); ++i)
		{
			if (raptor_ids.linear_search(i) != -1)
				json->setSlotVisible(i, false);
			else
				json->setSlotVisible(i, true);
		}
	}
	if (g_psd->hitTouchLayer("raptor", irr::EKS_RELEASED))
	{
		for (irr::u32 i = 0; i < json->getSlotCount(); ++i)
		{
			if (raptor_ids.linear_search(i) == -1)
				json->setSlotVisible(i, false);
			else
				json->setSlotVisible(i, true);
		}
	}
	if (g_psd->hitTouchLayer("ride", irr::EKS_RELEASED))
	{
		for (irr::u32 i = 0; i < json->getSlotCount(); ++i)
			json->setSlotVisible(i, true);
	}
	if (g_psd->hitTouchLayer("empty", irr::EKS_RELEASED))
	{
		for (irr::u32 i = 0; i < json->getSlotCount(); ++i)
			json->setSlotVisible(i, false);
	}
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
