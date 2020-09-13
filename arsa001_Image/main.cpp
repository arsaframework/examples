#include <arsa.h>

void init()
{
	g_psd->load("chars4_bg.psd");
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_EasyStart(  );

	return 0;
}
