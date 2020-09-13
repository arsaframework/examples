#include <arsa.h>

void init()
{
	g_psd->load("hit_img.psd");
}

void update()
{
	// hit with boss
	irr::s32 id = g_psd->hitTouchPrefix("boss", irr::EKS_RELEASED);
	if( id != -1 )
		g_psd->setPsdVisible(id, false);

	if( g_psd->hitTouchLayer("reset", irr::EKS_RELEASED ) )
	{
		irr::core::array<irr::s32> ids;
		g_psd->getPsdIdByPrefixs("boss", ids);
		for (irr::u32 i = 0; i < ids.size(); ++i)
			g_psd->setPsdVisible(ids[i], true);
	}
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart(  );

	return 0;
}
