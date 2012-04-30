#include "Core.hpp"
using namespace SIPL;

int main(int argc, char ** argv) {
    // Initialize SIPL (must always be called before using SIPL)
    Init();

    // Load image from disk and display it
    Image<color_uchar> * im = new Image<color_uchar>("images/lena.jpg");
    im->show();

    // Remove all the green from the color image im
    for(int i = 0; i < im->getWidth()*im->getHeight(); i++) {
        color_uchar p = im->get(i);
        p.green = 0;
        im->set(i, p);
    }

    // Save it
    im->save("test.png", "png");

    // Convert image to grayscale and display it
    Image<float> * im2 = new Image<float>(im);
    im2->show();

    // View the image using a custom level(0->5) and window(0->25)
    im2->show(0.5, 0.25);

    // Calculate the gradient of the image and display the vector field using colors
    Image<float2> * gradient = new Image<float2>(im2->getWidth(), im2->getHeight());
    Window<float2> * w = gradient->show();
    for(int x = 1; x < im2->getWidth()-1; x++) {
        for(int y = 1; y < im2->getHeight()-1; y++) {
            float2 vector;
            vector.x = 0.5*(im2->get(x+1,y)-im2->get(x-1,y));
            vector.y = 0.5*(im2->get(x,y+1)-im2->get(x,y-1));
            gradient->set(x,y,vector);
        }
        w->update(); // update the image on screen
    }

    // Load volume and display one slice on the screen
    // (Use arrow keys up and down to change the slice on screen)
    Volume<short> * v = new Volume<short>("Case00.mhd");
    v->show();
}
