#include <arsa.h>

// FSM
#define STATE_OPEN_FIRST	0
#define STATE_OPEN_SECOND	1
#define STATE_COMPARE		2
#define STATE_WIN			3
#define STATE_GAMEOVER		4

// open state
int state_open = STATE_OPEN_FIRST;

// image name, first and second
irr::core::stringc img_first;
irr::core::stringc img_second;

// array keep image by prefix
irr::core::array<irr::s32> all_imgs;

// score
int score = 0;

void init()
{
	// cache image
	g_psd->load("matching1.psd"); 

	// get all image prefix as img
	g_psd->getPsdIdByPrefixs("img", all_imgs);

	// hide all image
	for (irr::u32 i = 0; i < all_imgs.size(); ++i)
		g_psd->setPsdVisible(all_imgs[i], false);
}

// get hit id from touch
int getHitId()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		return g_psd->hitXY(x, y, true);
	}
	return -1;
}

void update()
{
	int hit_id = getHitId(); // get hit id from touch

	// FSM
	switch (state_open)
	{
		case STATE_OPEN_FIRST:
			{
				// check winner
				// algorithms
				// if all image visible then goto win state
				bool win = true;
				for (irr::u32 i = 0; i < all_imgs.size(); ++i)
				{
					if (!g_psd->isPsdVisible(all_imgs[i]))
						win = false;
				}
				if (win)
				{
					state_open = STATE_WIN;
					for (irr::u32 i = 0; i < all_imgs.size(); ++i)
					{
						g_psd->setPsdRot(all_imgs[i], irr::core::vector3df(0,1,0));
						g_psd->setPsdRotSpeed(all_imgs[i], 80);
					}
					break;
				}
				
				if (hit_id != -1)
				{
					// check hit_id is in array
					irr::s32 id = all_imgs.linear_search(hit_id);

					// if id is in array and invisible (mean don't open this image)
					if (id != -1 && !g_psd->isPsdVisible(all_imgs[id]) )
					{
						// keep first image
						img_first = g_psd->getPsdName(all_imgs[id]);

						// set visible
						g_psd->setPsdVisible(all_imgs[id], true);

						// go next state
						state_open = STATE_OPEN_SECOND;
					}
				}
			}
		break;
		case STATE_OPEN_SECOND:
			{
				if (hit_id != -1)
				{
					irr::s32 id = all_imgs.linear_search(hit_id);
					if (id != -1 && !g_psd->isPsdVisible(all_imgs[id]))
					{
						// keep second image
						img_second = g_psd->getPsdName(all_imgs[id]);
						g_psd->setPsdVisible(all_imgs[id], true);

						// next state
						state_open = STATE_COMPARE;
					}
				}
			}
		break;
		case STATE_COMPARE:
			{
				// token string from layer name.
				// algorithms
					// 1. layer name in photoshop MUST have prefix's name "img".
					// 2. use single space after prefix as name for compare e.g. "img mu".
					// 3. use arsa_TokenString() token string and keep to array.
					// e.g. "img mu"
					//		[0] = img
					//		[1] = mu
					// e.g. "img mu 2"
					//		[0] = img
					//		[1] = mu
					//		[1] = 2
					// 4. use array slot 1 for comparison.
				irr::core::array<irr::core::stringc> str_img_first, str_img_second;
				arsa_TokenString(img_first, str_img_first);
				arsa_TokenString(img_second, str_img_second);

				// equal image
				if (str_img_first[1] == str_img_second[1])
					score++;
				else // not equal
				{
					// check gameover
					// get id life by prefixs
					irr::core::array<irr::s32> lifes;
					g_psd->getPsdIdByPrefixs("life", lifes);

					// invisible life
					for (irr::u32 i = 0; i < lifes.size(); ++i)
					{
						if (g_psd->isPsdVisible(lifes[i]))
						{
							g_psd->setPsdVisible(lifes[i], false);
							break;
						}
					}

					// check life
					bool gameover = true;
					for (irr::u32 i = 0; i < lifes.size(); ++i)
					{
						if (g_psd->isPsdVisible(lifes[i]))
						{
							gameover = false;
							break;
						}
					}
					if (gameover) // goto gameover state
					{
						state_open = STATE_GAMEOVER;
						break;
					}

					// reset image pair
					int id1 = g_psd->getPsdId(img_first);
					int id2 = g_psd->getPsdId(img_second);
					g_psd->setPsdVisible(id1, false);
					g_psd->setPsdVisible(id2, false);

					// delay 800 millisec
					g_device->sleep(800);
				}

				// reset keep image name
				img_first = "";
				img_second = "";
				state_open = STATE_OPEN_FIRST;
			}
		break;
	} // state_open

	// touch reset button
	int reset = g_psd->getPsdId("reset");
	if (hit_id == reset)
	{
		// back to first state
		state_open = STATE_OPEN_FIRST;
		// reset score
		score = 0;
		// reset keep image names
		img_first = "";
		img_second = "";

		// reset all image
		for (irr::u32 i = 0; i < all_imgs.size(); ++i)
		{
			g_psd->setPsdVisible(all_imgs[i], false);
			g_psd->setPsdRot(all_imgs[i], irr::core::vector3df(0, 0, 0));
			g_psd->setPsdRotSpeed(all_imgs[i], 0);
		}

		// reset life
		// check gameover
		irr::core::array<irr::s32> lifes;
		g_psd->getPsdIdByPrefixs("life", lifes);
		for (irr::u32 i = 0; i < lifes.size(); ++i)
			g_psd->setPsdVisible(lifes[i], true);
	}
}

void hud()
{
	// draw score
	irr::core::stringw str;
	str = "Score: ";
	str += score;
	arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30), 10, 10, str);

	// get close id and draw on each images
	int img_close = g_psd->getPsdId("close");
	for (irr::u32 i = 0; i < all_imgs.size(); ++i)
	{
		float x, y;
		g_psd->getPsdPos(all_imgs[i], x, y);
		if(!g_psd->isPsdVisible(all_imgs[i]))
			g_psd->draw(img_close, x, y);
	}

	// draw text both win and gameover states
	switch (state_open)
	{
		case STATE_WIN:
			{
				arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30), 960/2, 50, "You Win!", irr::arsa::EDF_CENTER);
			}
		break;
		case STATE_GAMEOVER:
			{
				arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30), 960 / 2, 50, "GAMEOVER!", irr::arsa::EDF_CENTER, irr::video::SColor(255,255,0,0));
			}
		break;
	}
}

int main(int argc, char* args[])
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_SetFuncHud(hud);
	arsa_EasyStart( );

	return 0;
}
