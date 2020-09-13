#include <arsa.h>

#define STATE_STARTGAME		0
#define STATE_SELECT		1
#define STATE_UPDATE		2

int game_state = STATE_STARTGAME;
int state_num  = 0;
irr::core::stringc scene_prefix = "scene";

void init()
{
	g_psd->load("start.psd");
}

void update()
{
	switch (game_state)
	{
		case STATE_STARTGAME:
			{
				if (g_input->getTouchState(0) == irr::EKS_RELEASED)
				{
					int x = g_input->getTouchX(0);
					int y = g_input->getTouchY(0);

					int start = g_psd->getPsdId("start");
					if (start != -1)
					{
						if (g_psd->hitXYId(x, y, start))
						{
							g_psd->load("select.psd");
							game_state = STATE_SELECT;
						}
					}
				}
			} // STATE_STARTGAME
		break;
		case STATE_SELECT:
			{
				if (g_input->getTouchState(0) == irr::EKS_RELEASED)
				{
					int x = g_input->getTouchX(0);
					int y = g_input->getTouchY(0);

					// hit back
					int back = g_psd->getPsdIdByPrefix("back");
					if (back != -1)
					{
						if (g_psd->hitXYId(x, y, back))
						{
							g_psd->load("start.psd");
							game_state = STATE_STARTGAME;
							return;
						}
					}

					int state_hit = g_psd->hitXY(x, y);
					if (state_hit != -1)
					{
						irr::core::stringc name_hit = g_psd->getPsdName(state_hit);
						if (name_hit.find(scene_prefix.c_str()) != -1)
						{
							irr::core::stringc num = name_hit.subString(scene_prefix.size(), 1);
							state_num = atoi(num.c_str());
							g_psd->load(name_hit);
							game_state = STATE_UPDATE;
						}
					}
				}
			}
		break; // STATE_SELECT
		case STATE_UPDATE:
			{
				if (g_input->getTouchState(0) == irr::EKS_RELEASED)
				{
					int x = g_input->getTouchX(0);
					int y = g_input->getTouchY(0);

					int select = g_psd->getPsdIdByPrefix("select");
					int back = g_psd->getPsdIdByPrefix("Back");
					int next = g_psd->getPsdIdByPrefix("Next");

					int hit_id = g_psd->hitXY(x, y);
					if (hit_id == select)
					{
						g_psd->load("select.psd");
						game_state = STATE_SELECT;
					}
					else if (hit_id == back)
					{
						state_num--;
						irr::core::stringc state_name;
						state_name = scene_prefix;
						state_name += state_num;
						state_name += ".psd";
						g_psd->load(state_name);
					}
					else if (hit_id == next)
					{
						state_num++;
						irr::core::stringc state_name;
						state_name = scene_prefix;
						state_name += state_num;
						state_name += ".psd";
						g_psd->load(state_name);
					}

				}
			}
		break; // STATE_UPDATE
	} // switch
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart( "media352kmutt" );

	return 0;
}
