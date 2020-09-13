#include <arsa.h>

// video handle variable
irr::arsa::CAVSceneNode* video = 0;

void init()
{
	// allocate video handle, write at init
	video = arsa_AddAV();

	// open video file
	video->openFromFile( "combo.mpg" );

	// play video
	video->play();
}

void update()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		if( !video->isPlaying() )
			video->play();
	}
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit( init );
	arsa_SetFuncUpdate( update );
	arsa_EasyStart( );

	return 0;
}
