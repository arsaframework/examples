#include <arsa.h>

irr::arsa::CAVSceneNode* video = 0;

void init()
{
	video = arsa_AddAV( );
	video->openFromFile( "combo.mpg" );
	video->play();
	video->show(false);

	g_psd->load("video_psd.psd");
}

void hud()
{
	irr::s32 id = g_psd->getPsdId("video");
	irr::s32 bj = g_psd->getPsdId("bj");
	if (id != -1 && bj != -1 )
	{
		g_psd->draw(id, video->getTexture());
		g_psd->draw(bj,0);

		if (g_input->getTouchState(0) == irr::EKS_RELEASED)
		{
			if (!video->isPlaying())
				video->play();
		}
	}
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit( init );
	arsa_SetFuncHud( hud );
	arsa_EasyStart( );

	return 0;
}
