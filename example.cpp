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

    // View the image using a custom level(0.5) and window(0.25)
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
    Volume<uchar> * v = new Volume<uchar>("skull.raw", 256, 256, 256);
    v->show();
    v->show(100, X);
    v->show(100, X, 60, 100);
    v->show(60, 100);

    // Create and show maximum intensity projection (MIP) of the volume
    v->showMIP();
    v->showMIP(Y, 60, 100);

    // Convert volume to another data type
    Volume<float> * v2 = new Volume<float>(v);

    // Calculate 3D gradient of the volume
    Volume<float3> * vGradient = new Volume<float3>(v->getWidth(), v->getHeight(), v->getDepth());
    for(int x = 1; x < v->getWidth()-1; x++) {
        for(int y = 1; y < v->getHeight()-1; y++) {
            for(int z = 1; z < v->getDepth()-1; z++) {
                float3 vector;
                vector.x = 0.5*(v2->get(x+1,y,z)-v2->get(x-1,y,z));
                vector.y = 0.5*(v2->get(x,y+1,z)-v2->get(x,y-1,z));
                vector.z = 0.5*(v2->get(x,y,z+1)-v2->get(x,y,z-1));
                vGradient->set(x,y,z,vector);
            }
        }
    }
    vGradient->show();
    vGradient->save("test.raw");
}
