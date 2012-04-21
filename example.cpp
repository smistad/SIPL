#include "Core.hpp"
using namespace SIPL;

int main(int argc, char ** argv) {
    Init();

    Volume<uchar> volume = Volume<uchar>("CT_Patient_005.mhd");
    volume.show(128, Z, 70, 100);

    Image<float> imageTest = Image<float>(500, 500);
    for(int i = 0; i < imageTest.getWidth(); i++) {
        for(int j = 0; j < imageTest.getHeight(); j++) {
            imageTest.set(i,j,((float)(i+j)/(500+500)));
        }
    }
    imageTest.show(0.5, 0.5);

	Image<color_uchar> image = Image<color_uchar>("images/test.png");
    Window<color_uchar> w = image.show();
    for(int i = 0; i < image.getWidth(); i++) {
        for(int j = 0; j < image.getHeight(); j++) {
            color_uchar c = image.get(i,j);;
            if(c.green == 0) 
                c.green = 100;
            image.set(i,j,c);
        }
    w.update();
    }

	return 0;
}
