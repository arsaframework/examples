#include <arsa.h>

int score = 10;

void hud()
{
	// draw font by g_psd
	irr::core::stringw str = "Score: ";
	str += score;
	arsa_DrawFont(arsa_GetFont("TS-Prachachon-NP.ttf", 50), 10, 10, str, irr::arsa::EDF_DEFAULT, irr::video::SColor(255,0,0,255) );
	arsa_DrawFont(arsa_GetFont("Nithan.ttf", 100), "YOU WIN!", irr::core::recti(0, 0, 960, 640), irr::video::SColor(255, 255, 255, 100), true, true);
}

int main( int argc, char* args[] )
{
	arsa_SetFuncHud(hud);
	arsa_EasyStart( );

	return 0;
}
