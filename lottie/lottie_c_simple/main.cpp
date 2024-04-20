#include <arsa.h>

irr::arsa::CAVSceneNode* video = 0;
//irr::io::path fn = "79172-update.json";
//irr::io::path fn = "own.json";
//irr::io::path fn = "79965-website-and-app-development.json";
//irr::io::path fn = "79797-car-lottie-animation.json";
//irr::io::path fn = "sel/vocalno.json";
irr::io::path fn = "sel/bigbang.json";
//irr::io::path fn = "https://assets9.lottiefiles.com/packages/lf20_PBgNop.json";
//irr::io::path fn = "https://assets10.lottiefiles.com/private_files/lf30_hv93nakg.json";
//irr::io::path fn = "lf20_PBgNop.json";
irr::core::stringc guid;

void init()
{
	video = arsa_AddAV( );
	video->openFromFile( "combo.mpg" );
	//video->play();
	//video->show(false);	

	g_psd->load("video_psd.psd");
	
	//g_psd->setPsdTexture(id, video->getTexture());

	
	guid = g_lottie->Load(g_lottie->GetLottieThread(fn));
}

void update()
{
	if (g_lottie->IsReadyFirst(guid))
	{
		g_lottie->Play(guid);
		//g_lottie->SetFPS(guid, 1);
		//g_lottie->SetFPS(guid,120);
		//g_lottie->SetFPS(guid, 0.1f);
		//g_lottie->Loop(guid, irr::arsa::ELF_PINGPONG);
		//g_lottie->Loop(guid, irr::arsa::ELF_ONCE);
		g_lottie->Resize(guid, irr::core::dimension2du(512, 512));
		//g_lottie->Resize(guid, irr::core::dimension2du(64, 64));
		//g_lottie->Resize(guid, irr::core::dimension2du(4096, 4096));
		g_psd->setPsdVisible(g_psd->getPsdId("video"), true);
		g_psd->setPsdVisible(g_psd->getPsdId("loading"), false);
		g_psd->setPsdTexture(g_psd->getPsdId("video"), g_lottie->GetTexture(guid));
	}

	if (0&&g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		//g_lottie->Stop(guid);
		//g_lottie->Play(guid);
		if( g_lottie->IsPlaying(guid) )
			g_lottie->Pause(guid);
		else
			g_lottie->Play(guid);
	}

	if (g_input->isTouchLeft(0))
	{		
		irr::s32 f = g_lottie->GetFrame(guid);
		g_lottie->Stop(guid);
		f--;
		if (f <= 0)
			f = 0;
		g_lottie->SetFrame(guid, f);
	}
	if (g_input->isTouchRight(0))
	{
		irr::s32 f = g_lottie->GetFrame(guid);
		g_lottie->Stop(guid);
		f++;
		if (f >= g_lottie->GetFrameCount(guid))
			f = g_lottie->GetFrameCount(guid)-1;
		g_lottie->SetFrame(guid, f);
	}

	//video->avUpdate();
}
//
//void hud()
//{
//	irr::s32 id = g_psd->getPsdId("video");
//	irr::s32 bj = g_psd->getPsdId("bj");
//	if (id != -1 && bj != -1 )
//	{
//		g_psd->draw(id, video->getTexture());
//		g_psd->draw(bj,0);
//
//		if (g_input->getTouchState(0) == irr::EKS_RELEASED)
//		{
//			if (!video->isPlaying())
//				video->play();
//		}
//	}
//}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit( init );
	arsa_SetFuncUpdate(update);
	//arsa_SetFuncHud( hud );
	arsa_EasyStart( );

	return 0;
}
