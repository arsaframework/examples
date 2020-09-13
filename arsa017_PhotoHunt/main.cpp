#include <arsa.h>

#define STATE_HUNT			2
#define STATE_WIN			3
#define STATE_GAMEOVER		4

// open state
int state = STATE_HUNT;

// array keep image by prefix
irr::core::array<irr::s32> all_imgs;

// score
int score = 0;

void init()
{
	// cache image
	g_psd->load("hunt008.psd"); 

	// get all image prefix as img
	g_psd->getPsdIdByPrefixs("hint", all_imgs);

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
	int hit_id = getHitId();

	switch (state)
	{
		case STATE_HUNT:
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
				state = STATE_WIN;
				break;
			}

			if (hit_id == -1)
				break;

			// search hit id from array
			int id = all_imgs.linear_search(hit_id);

			// if id is in array and invisible (mean don't touch this hint)
			if (id != -1 && !g_psd->isPsdVisible(all_imgs[id]))
			{
				// get hint name
				irr::core::stringc name = g_psd->getPsdName(all_imgs[id]);

				// token string for get id from layer name
				irr::core::array<irr::core::stringc> str_img_first;
				arsa_TokenString(name, str_img_first);

				// visible second hint
				for (irr::u32 i = 0; i < all_imgs.size(); ++i)
				{
					// reject myself
					if (id == i)
						continue;

					// check layer id, if founded then visible it
					irr::core::stringc name2 = g_psd->getPsdName(all_imgs[i]);
					if (name2.find(str_img_first[1].c_str()) != -1)
					{
						g_psd->setPsdVisible(all_imgs[i], true);
						break;
					}
				}

				// visible first hint
				g_psd->setPsdVisible(all_imgs[id], true);				

				score++;
			}
			else // miss touch
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
				} // for i

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
					state = STATE_GAMEOVER;
					break;
				}
			} // miss touch
		}
		break; // case STATE_HUNT:
	} // switch (state)

	  // touch reset button
	int reset = g_psd->getPsdId("reset");
	if (hit_id == reset && hit_id != -1 && reset != -1)
	{
		state = STATE_HUNT;

		// reset score
		score = 0;

		// reset all image
		for (irr::u32 i = 0; i < all_imgs.size(); ++i)
			g_psd->setPsdVisible(all_imgs[i], false);

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
	arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),10, 10, str);

	// draw text both win and gameover states
	switch (state)
	{
		case STATE_WIN:
		{
			arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),960 / 2, 300, "You Win!", irr::arsa::EDF_CENTER);
		}
		break;
		case STATE_GAMEOVER:
		{
			arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),960 / 2, 300, "GAMEOVER!", irr::arsa::EDF_CENTER, irr::video::SColor(255, 255, 0, 0));
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
