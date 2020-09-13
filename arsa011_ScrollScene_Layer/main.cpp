#include <arsa.h>
#include <cdx/cdxlayer.h>

CDXLayer* layer = 0;

void init()
{
	layer = new CDXLayer();
	layer->Create("layer_2.jpg");
}

void update()
{
	layer->ScrollDown(1);
	layer->Draw();
}

void deinit()
{
	SAFEDELETE(layer);
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_SetFuncDeInit(deinit);
	arsa_EasyStart( );

	return 0;
}
