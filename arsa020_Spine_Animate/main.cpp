#include <arsa.h>

void init()
{
	irr::arsa::CARSAJson* json = arsa_AddJson();
	json->load("data/spineboy.json");
	json->setPosition(irr::core::vector3df(300,900,1));
	json->setAnimation("run");
}

int main( int argc, char* args[] )
{	
	arsa_SetPortrait();	
	arsa_SetFuncInit(init);
	arsa_EasyStart( );

	return 0;
}
