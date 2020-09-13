#include <arsa.h>

#define STATE_STARTGAME		0
#define STATE_SELECT		1
#define STATE_HUNT			2
#define STATE_WIN			3
#define STATE_GAMEOVER		4

// current state
int state_num = 0;

// scene prefix
irr::core::stringc scene_prefix = "hunt";

// open state
int state = STATE_STARTGAME;

// array keep image by prefix
irr::core::array<irr::s32> all_hints;

// keep state clear
irr::core::array<irr::s32> state_clears;

// score
int score = 0;

// state max 
int state_max = 0;

void init()
{
	g_fs->addFileArchive("arsa018_PhotoHunt_Advance.zip", false, false, irr::io::EFAT_ZIP, "sarosphotoworld");

	// start image
	g_psd->load("start.psd");
}

// get hit id from touch
int getHitId()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		return g_psd->hitXY(x, y);
	}
	return -1;
}

// query user tap on hint point
int isHitHint()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		irr::core::array<irr::s32> ids;
		g_psd->hitXYs(x, y, ids,true);
		for (irr::u32 i = 0; i < ids.size(); ++i)
		{
			irr::core::stringc name = g_psd->getPsdName(ids[i]);
			if (name.find("hint") != -1)
				return ids[i];
		}
	}
	return -1;
}

// open hint image, if hint_id = -1 mean open first invisible hint
void openHint(int hint_id)
{
	int id = hint_id;
	irr::core::stringc name;

	if (id != -1)
	{
		// visible first hint
		g_psd->setPsdVisible(id, true);

		// get hint name
		name = g_psd->getPsdName(id);
	}
	else
	{
		for (irr::u32 i = 0; i < all_hints.size(); ++i)
		{
			if (!g_psd->isPsdVisible(all_hints[i]))
			{
				name = g_psd->getPsdName(all_hints[i]);
				g_psd->setPsdVisible(all_hints[i], true);
				id = all_hints[i];
				break;
			}
		} // for i
	}

	// token string for get id from layer name
	irr::core::array<irr::core::stringc> str_img_first;
	arsa_TokenString(name, str_img_first);

	// visible second hint
	for (irr::u32 i = 0; i < all_hints.size(); ++i)
	{
		// reject myself
		if (id == all_hints[i])
			continue;

		// check layer id, if founded then visible it
		irr::core::stringc name2 = g_psd->getPsdName(all_hints[i]);
		if (name2.find(str_img_first[1].c_str()) != -1)
		{
			g_psd->setPsdVisible(all_hints[i], true);
			break;
		}
	}
}

// update all
void update()
{
	int hit_id = getHitId();

	switch (state)
	{
		case STATE_STARTGAME:
		{
			if (g_input->getTouchRepeatCount() == 2)
			{
				int x = g_input->getTouchX(0);
				int y = g_input->getTouchY(0);
				int hit = g_psd->hitXY(x, y);

				int start = g_psd->getPsdId("start");
				if (hit == start && hit != -1 && start != -1)
				{
					state_clears.clear();
					g_psd->load("select.psd");
					state = STATE_SELECT;

					// get max state
					irr::core::array<irr::s32> states;
					g_psd->getPsdIdByPrefixs(scene_prefix, states);
					state_max = states.size();
				}
			}
		} // STATE_STARTGAME
		break;
		case STATE_SELECT:
		{
			if (g_input->getTouchState(0) == irr::EKS_RELEASED)
			{
				// hit back
				int back = g_psd->getPsdIdByPrefix("back");
				if (hit_id == back && hit_id != -1 && back != -1)
				{
					score = 0;
					state_clears.clear( );
					g_psd->load("start.psd");
					state = STATE_STARTGAME;
					break;
				}

				if (hit_id != -1)
				{
					irr::core::stringc name_hit = g_psd->getPsdName(hit_id);
					if (name_hit.find(scene_prefix.c_str()) != -1)
					{
						irr::core::stringc num = name_hit.subString(scene_prefix.size(), 3);
						state_num = atoi(num.c_str());

						state = STATE_HUNT;

						// cache image
						g_psd->load(name_hit);

						// get all image prefix as img
						all_hints.clear();
						g_psd->getPsdIdByPrefixs("hint", all_hints);//*/
					}
				}
			}
		}
		break; // STATE_SELECT

		case STATE_HUNT:
		{
			// check winner
			// algorithms
				// if all image visible then goto win state
			bool win = true;
			for (irr::u32 i = 0; i < all_hints.size(); ++i)
			{
				if (!g_psd->isPsdVisible(all_hints[i]))
					win = false;
			}
			if (win)
			{
				// clear state
				if( state_clears.linear_search(state_num) == -1 )
					state_clears.push_back(state_num);

				// get panel group
				int g_panel = g_psd->getGroupId("g_panel");
				g_psd->setGroupVisible(g_panel);

				if (state_clears.size() >= state_max )
				{
					int next = g_psd->getPsdIdByPrefix("next");
					g_psd->setPsdVisible(next, false);
				}

				state = STATE_WIN;
				break;
			}

			// reject if dont hit any thing
			if (hit_id == -1)
				break;

			// search hit id from array
			int id = isHitHint();

			// if id founded and still visible (mean don't touch this hint)
			if(id != -1 && !g_psd->isPsdVisible(id))
			{				
				score++;
				openHint(id);
			}
			else // miss touch
			{
				if (hit_id != -1)
				{
					irr::core::stringc name = g_psd->getPsdName(hit_id);
					if (name.find("answer") != -1)
					{
						g_psd->setPsdVisible(hit_id, false);
						openHint(-1);
						break;
					}
				}

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
					// get panel group
					int g_panel = g_psd->getGroupId("g_panel");					
					g_psd->setGroupVisible(g_panel);
					int next = g_psd->getPsdIdByPrefix("next");
					g_psd->setPsdVisible(next,false);

					state = STATE_GAMEOVER;
					break;
				}
			} // miss touch
		}
		break; // case STATE_HUNT:
		case STATE_WIN:
		{
			int next = g_psd->getPsdIdByPrefix("next");
			if (hit_id == next && hit_id != -1 && next != -1)
			{
				state = STATE_HUNT;

				bool ok = false;
				while(1)
				{
					// clamp state
					state_num = (++state_num)%(state_max+1);
					if (state_num == 0)
						state_num = 1;
					int id = state_clears.linear_search(state_num);
					if (id == -1)
						break;
				}
				//state_num++;
				irr::core::stringc name = scene_prefix;
				name += state_num;
				name += ".psd";
				g_psd->load(name);//*/

				// get all image prefix as img
				all_hints.clear();
				g_psd->getPsdIdByPrefixs("hint", all_hints);//*/
			}
		}
		break;
	} // switch (state)

	// touch reset button
	int reset = g_psd->getPsdIdByPrefix("reset");
	if (hit_id == reset && hit_id != -1 && reset != -1)
	{
		// reset score
		if (state == STATE_GAMEOVER)
			score = 0;

		state = STATE_HUNT;

		// hind get panel group
		int g_panel = g_psd->getGroupId("g_panel");
		g_psd->setGroupVisible(g_panel, false);

		// reset all image
		for (irr::u32 i = 0; i < all_hints.size(); ++i)
			g_psd->setPsdVisible(all_hints[i], false);

		// reset life
		irr::core::array<irr::s32> lifes;
		g_psd->getPsdIdByPrefixs("life", lifes);
		for (irr::u32 i = 0; i < lifes.size(); ++i)
			g_psd->setPsdVisible(lifes[i], true);

		// reset answers
		irr::core::array<irr::s32> answers;
		g_psd->getPsdIdByPrefixs("answer", answers);
		for (irr::u32 i = 0; i < answers.size(); ++i)
			g_psd->setPsdVisible(answers[i], true);
	}

	// touch select state
	irr::core::array<irr::s32> select_ids;
	int select = g_psd->getPsdIdByPrefixs("select", select_ids);
	int id = select_ids.linear_search(hit_id);
	if (id != -1)
	{
		// reset score
		if (state == STATE_GAMEOVER)
			score = 0;

		g_psd->load("select.psd");
		state = STATE_SELECT;
		for (irr::u32 i = 0; i < state_clears.size(); ++i)
		{
			irr::core::stringc name = scene_prefix;
			name += state_clears[i];
			name += ".psd";
			int id = g_psd->getPsdId(name);
			if (id != -1)
				g_psd->setPsdVisible(id,false);
		}
	}
}

void hud()
{
	if (state != STATE_STARTGAME)
	{
		// draw score
		irr::core::stringw str;
		str = "Score: ";
		str += score;
		arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),10, 10, str);
	}

	// draw text both win and gameover states
	switch (state)
	{
		case STATE_WIN:
		{
			arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),(960 / 2)+20, 200, "You Win!", irr::arsa::EDF_CENTER);
		}
		break;
		case STATE_GAMEOVER:
		{
			arsa_DrawFont(arsa_GetFont("Nithan.ttf", 30),(960 / 2)+20, 250, "GAME OVER!", irr::arsa::EDF_CENTER, irr::video::SColor(255, 255, 0, 0));
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
