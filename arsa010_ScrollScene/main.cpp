#include <arsa.h>

void init()
{
	g_psd->load("bg001.psd"); // cache image

	// cache snd
	g_snd->Load("c001_12.wav");
	g_snd->Load("c011_12.wav");
}

void update()
{
	float speed = 20; // move speed

	// get current offset
	irr::core::vector3df offset = g_psd->getOffset();

	// translate x, y offset by touch input
	if (g_input->isTouchRight(0))
		offset.X += speed;
	else if (g_input->isTouchLeft(0))
		offset.X -= speed;
	if (g_input->isTouchUp(0))
		offset.Y -= speed;
	else if (g_input->isTouchDown(0))
		offset.Y += speed;

	// set to psd image
	g_psd->setOffset(offset);

	// play snd if touch at char
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		if (g_psd->hitXYLayer(x, y, "mu"))
			g_snd->Play("c001_12.wav");
		if (g_psd->hitXYLayer(x, y, "exact"))
			g_snd->Play("c011_12.wav");
	}
}

int main(int argc, char* args[])
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart();

	return 0;
}
