#include "SIPL/Core.h"
#include "SIPL/Masks.h"
using namespace SIPL;

int main(int argc, char ** argv)
{
    Init();

	Image image = Image("images/example.bmp");
	image.show();

    /*
	Image edgeImage = image.convolution(new SobelYMask());
	edgeImage.show();
	edgeImage.save("images/example_result.bmp", "bmp");
    */

	Image blurredImage = image.convolution(new AverageMask(5));
	Window w = blurredImage.show();
    for(int i = 0; i < 150;i++) {
        for(int j = 0; j < 150; j++) {
            blurredImage.getPixel(i,j).set(0);
        }
    }
    w.update();
    blurredImage = blurredImage.convolution(new AverageMask(7));
    blurredImage.show();

	return 0;
}
