#include "Visualization.hpp"
#include <iostream>
using namespace SIPL;

int Visualization::windowCounter = 0;

Visualization::Visualization(BaseDataset * image) {
    if(images.size() == 3)
        throw SIPLException("A visualization can only contain a maximum of 3 images/volumes.");
    images.push_back(image);
    setLevel(0.5);
    setWindow(1.0);
    isVolumeVisualization = image->isVolume;
    if(isVolumeVisualization) {
        direction = Z;
        slice = image->getSize().z / 2;
    }
}

Visualization::Visualization(BaseDataset * image, BaseDataset * image2) {

    // Check that both are either image or volume
    if(!(image->isVolume == image2->isVolume))
        throw SIPLException("A visualization can only contain images or volumes, not both.");
    if(image->isVectorType && image2->isVectorType)
        throw SIPLException("A visualization can not contain more than one vector image/volume.");
    if(images.size() > 1)
        throw SIPLException("A visualization can only contain a maximum of 3 images/volumes.");
    if(!(image->getSize() == image2->getSize()))
        throw SIPLException("Images/volumes must be of same size to be visualized together.");

    images.push_back(image);
    images.push_back(image2);
    setLevel(0.5);
    setWindow(1.0);
    isVolumeVisualization = image->isVolume;
    if(isVolumeVisualization) {
        direction = Z;
        slice = image->getSize().z / 2;
    }
}

Visualization::Visualization(BaseDataset * image, BaseDataset * image2, BaseDataset * image3) {

    // Check that both are either image or volume
    if(!(image->isVolume == image2->isVolume && image->isVolume == image3->isVolume))
        throw SIPLException("A visualization can only contain images or volumes, not both.");
    if(image->isVectorType && image2->isVectorType && image3->isVectorType)
        throw SIPLException("A visualization can not contain more than one vector image/volume.");
    if(images.size() > 0)
        throw SIPLException("A visualization can only contain a maximum of 3 images/volumes.");
    if(!(image->getSize() == image2->getSize() && image->getSize() == image3->getSize()))
        throw SIPLException("Images/volumes must be of same size to be visualized together.");

    images.push_back(image);
    images.push_back(image2);
    images.push_back(image3);
    setLevel(0.5);
    setWindow(1.0);
    isVolumeVisualization = image->isVolume;
    if(isVolumeVisualization) {
        direction = Z;
        slice = image->getSize().z / 2;
    }
}

uchar levelWindow(float value, float level, float window) {
    float result;
    if(value < level-window*0.5f) {
        result = 0.0f;
    } else if(value > level+window*0.5f) {
        result = 1.0f;
    } else {
        result = (float)(value-(level-window*0.5f)) / window;
    }
    result = SIPL::round(result*255);
    return result;
}

void Visualization::renderSlice(int imageNr, GdkPixbuf * pixBuf) {
    int3 size = images[0]->getSize();
    int xSize = 0;
    int ySize = 0;
    switch(direction) {
        case X:
            // x direction
            xSize = size.y;
            ySize = size.z;
            break;
        case Y:
            // y direction
            xSize = size.x;
            ySize = size.z;
            break;
        case Z:
            // z direction
            xSize = size.x;
            ySize = size.y;
            break;
    }

    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    int n =  gdk_pixbuf_get_n_channels(pixBuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixBuf);
    float * floatData = images[imageNr]->getFloatData();
    for(int y = 0; y < ySize; y++) {
    for(int x = 0; x < xSize; x++) {
        float intensity;
        switch(direction) {
            case X:
                intensity = floatData[slice + x*size.x + (ySize-1-y)*size.x*size.y];
                break;
            case Y:
                intensity = floatData[x + slice*size.x + (ySize-1-y)*size.x*size.y];
                break;
            case Z:
                intensity = floatData[x + y*size.x + slice*size.x*size.y];
                break;
        }
        int i = x*n+y*rowstride;
        guchar * p = pixels + i;
        if(images.size() == 1) {
            p[0] = levelWindow(intensity, level[images[imageNr]], window[images[imageNr]]);
            p[1] = p[0];
            p[2] = p[0];
        } else {
            p[imageNr] = levelWindow(intensity, level[images[imageNr]], window[images[imageNr]]);
        }
   }}
   delete[] floatData;
}

void Visualization::renderImage(int imageNr, GdkPixbuf * pixBuf) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    int n =  gdk_pixbuf_get_n_channels(pixBuf);

    float * floatData = images[imageNr]->getFloatData();
    for(int i = 0; i < images[imageNr]->getTotalSize(); i++) {
        guchar * p = pixels + i * n;
        if(images.size() == 1) {
            p[0] = levelWindow(floatData[i], level[images[imageNr]], window[images[imageNr]]);
            p[1] = p[0];
            p[2] = p[0];
        } else {
            p[imageNr] = levelWindow(floatData[i], level[images[imageNr]], window[images[imageNr]]);
        }
    }
    delete[] floatData;
}

GdkPixbuf * Visualization::render() {
    int3 size = images[0]->getSize();
    int xSize = 0;
    int ySize = 0;
    if(isVolumeVisualization) {
        switch(direction) {
            case X:
                // x direction
                xSize = size.y;
                ySize = size.z;
                break;
            case Y:
                // y direction
                xSize = size.x;
                ySize = size.z;
                break;
            case Z:
                // z direction
                xSize = size.x;
                ySize = size.y;
                break;
        }
    } else {
        xSize = size.x;
        ySize = size.y;
    }

    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, xSize,ySize));

    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
    for(int i = 0; i < images.size(); i++) {
        if(isVolumeVisualization) {
            renderSlice(i, pixBuf);
        } else {
            renderImage(i, pixBuf);
        }
    }
    return pixBuf;
}

void SIPL::Visualization::setLevel(float level) {
    for(int i = 0; i < images.size(); i++) {
        setLevel(images[i], level);
    }
}

void SIPL::Visualization::setWindow(float window) {
    for(int i = 0; i < images.size(); i++) {
        setWindow(images[i], window);
    }
}

void SIPL::Visualization::setLevel(BaseDataset* image, float level) {
    this->level[image] = level;
}

void SIPL::Visualization::setWindow(BaseDataset* image, float window) {
    this->window[image] = window;
}

void Visualization::display() {
    // For all images, get float data and then visualize it using window and level
    // Create image widget and fill up the pixbuf
    int3 size = images[0]->getSize();
    gdk_threads_enter();

    GdkPixbuf * pixBuf = render();

    // Create GUI
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Visualization::windowCounter++;
    char * title = new char[20];
    sprintf(title, "Visualization #%d", Visualization::windowCounter);
    gtk_window_set_title(GTK_WINDOW(window), title);
	GtkWidget * toolbar = gtk_toolbar_new();
	gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);

	gtk_window_set_default_size(
			GTK_WINDOW(window),
			size.x,
			size.y + 35
	);

    GtkWidget * vbox = gtk_vbox_new(FALSE, 1);

	gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar,FALSE,FALSE,0);
    GtkWidget * scaledImage = gtk_image_new_from_pixbuf(pixBuf);
    GtkWidget *  scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport((GtkScrolledWindow *)scrolledWindow, scaledImage);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow,TRUE,TRUE,0);
	gtk_widget_show_all(window);

	gdk_threads_leave();
}
