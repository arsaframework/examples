#include <arsa.h>

#define LIGHT_OFF	0
#define LIGHT_ON	1
int light_state = LIGHT_OFF;

void init()
{
	g_psd->load("image.psd");
}

void update()
{
	if (light_state == LIGHT_OFF)
	{
		g_psd->setPsdVisible(g_psd->getPsdId("on"), false);
		g_psd->setPsdVisible(g_psd->getPsdId("off"), true);
		g_psd->setPsdVisible(g_psd->getPsdId("dark"), true);
		if (g_psd->hitTouchLayer("off", irr::EKS_RELEASED))
			light_state = LIGHT_ON;
	} // if (light_state == LIGHT_OFF)
	else if (light_state == LIGHT_ON)
	{
		g_psd->setPsdVisible(g_psd->getPsdId("on"), true);
		g_psd->setPsdVisible(g_psd->getPsdId("off"), false);
		g_psd->setPsdVisible(g_psd->getPsdId("dark"), false);
		if (g_psd->hitTouchLayer("on", irr::EKS_RELEASED))
			light_state = LIGHT_OFF;
	} // if (light_state == LIGHT_OFF)

}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
