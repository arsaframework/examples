#include <arsa.h>

// header converter string to unicode
#include <irrUString.h>

// x, y axis use as position of credit text
int x = 10, y = 10;
// string buffer
irr::core::stringw credit;

void init()
{
	// open file from storage
	// if user want use other language e.g. thai
	// please change encoding to UTF8 before saving.
	irr::io::IReadFile* fp = g_fs->createAndOpenFile("credit.txt");
	if (fp)
	{
		// read file to buffer
		// declare buffer as array u8
		irr::core::array<irr::u8> dats;
		// allocate dynamic array by file size
		dats.set_used(fp->getSize());
		// read file to our buffer
		fp->read(dats.pointer(), fp->getSize());
		// drop file pointer
		fp->drop();		

		// convert utf8 to utf16 by ustring
		irr::core::ustring stru(dats.const_pointer());
		credit = stru.toUTF16().const_pointer();
	}

	// load ttf font
	//g_psd->loadFont("Nithan.ttf", 50);
}

void hud()
{	
	// declare text scroll speed
	// 500 unit multiply with delta time
	// because we want support this speed in every device
	irr::f32 speed = 500 * g_psd->getDeltaTime();

	// check input touch up and down
	// up y axis is decrease, 
	// down y axis is increase
	if (g_input->isTouchUp(0))
		y -= speed;
	if (g_input->isTouchDown(0))
		y += speed;

	// limit moving axis
	if (y > 0) y = 0;
	if (y < -440)y = -440;

	// draw font
	arsa_DrawFont(arsa_GetFont("Nithan.ttf", 50),x, y, credit);

	// print axis x y
	irr::core::stringc axis("x:");
	axis += x;
	axis += " y:";
	axis += y;
	arsa_DrawFont(arsa_GetFont("Nithan.ttf", 50),750, 10, axis);
}

int main( int argc, char* args[] )
{
	arsa_SetFuncInit(init);
	arsa_SetFuncHud(hud);
	arsa_EasyStart( );

	return 0;
}
