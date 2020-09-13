#include <arsa.h>

int score = 0;
irr::gui::IGUIFont* font1 = 0;
irr::gui::IGUIFont* font2 = 0;

void init()
{
	g_psd->load("game_move.psd");
	font1 = arsa_GetFont( "TS-Prachachon-NP.ttf", 50 );
	font2 = arsa_GetFont( "Nithan.ttf", 100 );
	arsa_DrawFont(font2,"YOU WIN!", irr::core::recti(0,0,960,640), irr::video::SColor(255,0,0,255), true, true );
	g_snd->Load( "hit.wav" ); // cache snd
	g_snd->Play("endtitle.wma",0,true); // play bg snd
}

void update()
{
	irr::s32 mu = g_psd->getPsdId( "mu" );
	if( mu != -1 )
	{
		irr::f32 x,y;
		irr::f32 speed = 600*g_psd->getDeltaTime();
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

		irr::s32 id = g_psd->hitId( mu );
		if( id != -1 && g_psd->getPsdName(id).find("boss") != -1 )
		{
			score++;
			g_psd->setPsdVisible( id, false );			
			if(score==10)
				g_snd->Play( "win.wav" );
			else
				g_snd->Play( "hit.wav" );
		}
	}

	if( g_input->getTouchState(0) == irr::EKS_RELEASED )
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if( g_psd->hitXYLayer(x,y,"reset") )
		{
			score = 0;
			for( irr::u32 i = 0; i < g_psd->getPsdCount(); ++i )
			{
				if( g_psd->getPsdName(i).find("boss") != -1 )
					g_psd->setPsdVisible(i,true);
			}
		}
	}
}

void hud()
{
	irr::core::stringw str = "Score: ";
	str += score;
	arsa_DrawFont( font1, 10, 10, str, irr::arsa::EDF_DEFAULT, irr::video::SColor(255,0,0,255) );

	if( score == 10 )
		arsa_DrawFont(font2, "YOU WIN!", irr::core::recti(0,0,960,640), irr::video::SColor(255,0,0,255), true, true );
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_SetFuncHud(hud);
	arsa_EasyStart( );

	return 0;
}
