#include <arsa.h>

irr::arsa::CARSAJson* json = 0;

irr::core::array<irr::s32> ids;
irr::f32 scale = 1;
irr::f32 z_rot = 0;

void init()
{
	json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,960,1));
	json->setAnimation("walk");

	// get slot depend on head
	ids.push_back(json->getSlotId("head"));
	ids.push_back(json->getSlotId("neck"));
	ids.push_back(json->getSlotId("mouth"));
	ids.push_back(json->getSlotId("goggles"));
	ids.push_back(json->getSlotId("eye"));
	ids.push_back(json->getSlotId("muzzle"));

	// set all to manual control
	for (irr::u32 i = 0; i < ids.size(); ++i)
		json->setSlotManual(ids[i], true);		

	g_psd->load("transform.psd");
	g_psd->setDrawOrder(1);
}

void update()
{
	irr::f32 speed = 10 * g_psd->getDeltaTime();
	if (g_psd->hitTouchLayer("scale_inc", irr::EKS_DOWN))
		scale += speed;
	if (g_psd->hitTouchLayer("scale_dec", irr::EKS_DOWN))
		scale -= speed;
	if (g_psd->hitTouchLayer("rot_inc", irr::EKS_DOWN))
		z_rot += speed*20;
	if (g_psd->hitTouchLayer("rot_dec", irr::EKS_DOWN))
		z_rot -= speed*20;

	// clamp scale 0.2..2.5
	scale = irr::core::clamp(scale, 0.2f, 2.5f);

	// clamp pingpong rotation 0..360 
	z_rot = arsa_clamp_float(z_rot, 0, 360); 

	irr::core::vector3df pos = json->getBonePos("neck");
	for (irr::u32 i = 0; i < ids.size(); ++i)
	{
		json->setSlotPos(ids[i], pos);		
		json->setSlotRot(ids[i], irr::core::vector3df(0,0,z_rot));
		json->setSlotScale(ids[i], irr::core::vector3df(scale));
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
