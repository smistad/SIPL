#include "SIPL/Core.h"
using namespace SIPL;

int main(int argc, char ** argv)
{
	atexit(quit);

	Image * image = new Image("images/example.bmp");
	image->show();

	Image * edgeImage = image->convolution(new LaplaceMask());
	edgeImage->show();
	edgeImage->save("images/example_result.bmp", "bmp");

	Image * blurredImage = image->convolution(new AverageMask(5));
	blurredImage->show();

	return 0;
}
