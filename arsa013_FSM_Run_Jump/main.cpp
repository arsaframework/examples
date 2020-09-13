#include <arsa.h>

#define CHAR_IDLE		0
#define CHAR_RUN_RIGHT	1
#define CHAR_RUN_LEFT	2
#define CHAR_JUMP_UP	3
#define CHAR_JUMP_DOWN	4

int mu_state = CHAR_IDLE; // run state
int mu_jump_state = CHAR_IDLE; // jump state

int mu_group = -1; // mu group animation

// jump parameters
float mu_jump_speed = 10;
float mu_jump_max = 150;
float mu_jump_count = 0;

irr::core::vector3df mu_pos; // mu position
irr::core::dimension2du mu_size; // mu size in pixel

void init()
{
	// cache image
	g_psd->load("bg001_mu.psd");
	
	// get start pos
	int start_pos = g_psd->getPsdId("start_pos");
	g_psd->getPsdPos(start_pos, mu_pos.X, mu_pos.Y);

	// get char group
	mu_group = g_psd->getGroupId("g_mu");
	g_psd->setGroupPos(mu_group, mu_pos.X, mu_pos.Y);

	// get size of clamp pos when run hit wall
	mu_size = g_psd->getPsdSize(g_psd->getPsdId("mu_run"));
}

// check user tap left button
bool update_StateLeft()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		int left = g_psd->getPsdIdByPrefix("arrow_left");
		if (left != -1)
		{
			if (g_psd->hitXYId(x, y, left))
			{
				mu_state = CHAR_RUN_LEFT;
				g_psd->setGroupVisibleOne(mu_group, g_psd->getPsdId("mu_run"), true);
				g_psd->setGroupPlayOne(mu_group, g_psd->getPsdId("mu_run"), true);
				g_psd->setGroupDrawFlags(mu_group, irr::arsa::EDF_HFLIP | irr::arsa::EDF_CENTER | irr::arsa::EDF_BOTTOM);
				return true;
			}
		}
	}

	return false;
}

// check user tap right button
bool update_StateRight()
{
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);
		int right = g_psd->getPsdIdByPrefix("arrow_right");
		if (right != -1)
		{
			if (g_psd->hitXYId(x, y, right))
			{
				mu_state = CHAR_RUN_RIGHT;
				g_psd->setGroupVisibleOne(mu_group, g_psd->getPsdId("mu_run"), true);
				g_psd->setGroupPlayOne(mu_group, g_psd->getPsdId("mu_run"), true);
				g_psd->setGroupDrawFlags(mu_group, irr::arsa::EDF_CENTER | irr::arsa::EDF_BOTTOM);
				return true;
			}
		}
	}

	return false;
}

// update run state
void update_Run()
{
	irr::f32 speed = 10;
	switch (mu_state)
	{
		case CHAR_IDLE:
		{
			update_StateLeft();
			update_StateRight();
		}
		break; // idle

		case CHAR_RUN_LEFT:
		{
			// exit state
			irr::core::dimension2du size = mu_size / 2;
			if (mu_pos.X <= size.Width)
			{
				mu_state = CHAR_IDLE;
				if (mu_jump_state == CHAR_IDLE)
				{
					g_psd->setGroupVisibleOne(mu_group, g_psd->getPsdId("mu"), true);
					g_psd->setGroupPlayOne(mu_group, g_psd->getPsdId("mu"), true);
				}
				break;
			}

			// change state
			update_StateRight();

			// move scene
			irr::core::vector3df offset = g_psd->getOffset();
			offset.X -= speed;
			g_psd->setOffset(offset);

			// move char
			mu_pos.X -= speed;
		}
		break; // left

		case CHAR_RUN_RIGHT:
		{
			// exit state
			irr::core::dimension2du size = mu_size / 2;
			irr::core::aabbox3df scene_max = g_psd->getMaxOffset();
			if (mu_pos.X >= scene_max.MaxEdge.X - size.Width)
			{
				mu_state = CHAR_IDLE;
				if (mu_jump_state == CHAR_IDLE)
				{
					g_psd->setGroupVisibleOne(mu_group, g_psd->getPsdId("mu"), true);
					g_psd->setGroupPlayOne(mu_group, g_psd->getPsdId("mu"), true);
				}
				break;
			}

			// change state
			update_StateLeft();

			irr::core::dimension2du screen = g_driver->getScreenSize();

			// move scene
			irr::core::vector3df offset = g_psd->getOffset();
			offset.X += speed;
			g_psd->setOffset(offset);

			// move char
			mu_pos.X += speed;
		} 
		break; // right
	} // switch
}
void update_Jump()
{
	// move scene
	irr::core::vector3df offset = g_psd->getOffset();

	switch (mu_jump_state)
	{
		case CHAR_IDLE:
			{
				if (g_input->getTouchState(0) == irr::EKS_PRESSED)
				{
					int x = g_input->getTouchX(0);
					int y = g_input->getTouchY(0);

					int bj = g_psd->getPsdIdByPrefix("button_jump");
					if (bj != -1)
					{
						if (g_psd->hitXYId(x, y, bj))
						{
							mu_jump_state = CHAR_JUMP_UP;
							mu_jump_count = 0;
							g_psd->setGroupVisibleOne(mu_group, g_psd->getPsdId("mu_jump"), true);
							g_psd->setGroupPlayOne(mu_group, g_psd->getPsdId("mu_jump"),true);
						}
					}
				}
			}
		break;
		case CHAR_JUMP_UP:
			{
				irr::f32 speed = mu_jump_speed;

				// exit state
				if (mu_jump_count >= mu_jump_max)
				{
					mu_jump_state = CHAR_JUMP_DOWN;
					break;
				}

				mu_jump_count += speed;

				// move char
				mu_pos.Y -= speed;

				offset.Y -= speed;
			}
		break;
		case CHAR_JUMP_DOWN:
		{
			irr::f32 speed = mu_jump_speed;

			// exit state
			if (mu_jump_count <= 0)
			{
				mu_jump_count = 0;
				mu_jump_state = CHAR_IDLE;

				irr::core::stringc psd_name;
				if (mu_state == CHAR_IDLE)
					psd_name = "mu";
				else
					psd_name = "mu_run";
				int psd_id = g_psd->getPsdId(psd_name);
				g_psd->setGroupVisibleOne(mu_group, psd_id, true);
				g_psd->setGroupPlayOne(mu_group, psd_id, true);
				break;
			}

			mu_jump_count -= speed;

			// move char
			mu_pos.Y += speed;
			offset.Y += speed;
		}
		break;
	} // switch (mu_state)

	g_psd->setOffset(offset);
}

void update()
{
	update_Run();
	update_Jump();

	// final set mu pos
	g_psd->setGroupPos(mu_group, mu_pos.X, mu_pos.Y);
}

int main(int argc, char* args[])
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_EasyStart();

	return 0;
}
