#include <arsa.h>

void init()
{
	irr::arsa::CARSAJson* json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(320,900,1));
}

int main( int argc, char* args[] )
{
	arsa_SetPortrait();
	arsa_SetFuncInit(init);
	arsa_EasyStart( );

	return 0;
}
