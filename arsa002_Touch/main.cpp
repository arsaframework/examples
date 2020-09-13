#include <arsa.h>

void init()
{
	g_psd->load("move_img.psd");
}

void update()
{
	irr::s32 mu = g_psd->getPsdId( "mu" );
	if( mu != -1 )
	{
		irr::f32 x,y;
		irr::f32 speed = 500*g_psd->getDeltaTime();
		g_psd->getPsdPos(mu,x,y);
		if( g_input->isTouchLeft(0) )
			x -= speed;
		if( g_input->isTouchRight(0) )
			x += speed;
		if( g_input->isTouchUp(0) )
			y -= speed;
		if( g_input->isTouchDown(0) )
			y += speed;
		g_psd->setPsdPos(mu,x,y);
	}
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( );

	return 0;
}
