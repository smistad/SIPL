#include "Core.hpp"
using namespace SIPL;

int main(int argc, char ** argv) {
    // Initialize SIPL
    Init();

    // Load image from disk and display it
    Image<float> im("images/lena.jpg");
    im.show();

    // Create image, manipulate the pixels and save it to disk

    // Calculate the gradient of the image and display the vector field using colors
    Image<float2> gradient(im.getWidth(), im.getHeight());
    for(int x = 0; x < im.getWidth()-1; x++) {
        for(int y = 1; y < im.getHeight()-1; y++) {
            float2 vector;
            vector.x = 0.5*(im.get(x+1,y)-im.get(x-1,y));
            vector.y = 0.5*(im.get(x,y+1)-im.get(x,y-1));
            gradient.set(x,y,vector);
        }
    }
    gradient.show();

    // Load volume and display one slice on the screen
    Volume<uchar> v("CT_Patient_005.mhd");
    v.show();

    End();
}
