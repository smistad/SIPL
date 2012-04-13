#include "SIPL/Core.h"
using namespace SIPL;

int main(int argc, char ** argv)
{
    Init();

	Image<PIXEL_FLOAT> image = Image<PIXEL_FLOAT>("images/example.bmp");
    for(int i = 0; i < 50; i++) {
    for(int j = 0; j < 50; j++) {
        image.getPixel(i,j).set(0.5f);
    }}
	image.show();

	return 0;
}
