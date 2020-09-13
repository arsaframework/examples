#include <arsa.h>

void init()
{
	g_snd->Play( "win.wav" );
	g_snd->Play("hit.wav");
	g_snd->Play( "endtitle.wma" );
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_EasyStart( );

	return 0;
}
