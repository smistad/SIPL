#include "Visualization.hpp"
#include <iostream>
using namespace SIPL;

int Visualization::windowCounter = 0;

Visualization::Visualization(BaseDataset * image) {
    if(images.size() == 3)
        throw SIPLException("A visualization can only contain a maximum of 3 images/volumes.");
    images.push_back(image);
    size = image->getSize();
    setLevel(0.5);
    setWindow(1.0);
    scale = 1.0f;
    angle = 0.5f*M_PI;
    isVolumeVisualization = image->isVolume;
    type = SLICE;
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
    size = image->getSize();
    setLevel(0.5);
    setWindow(1.0);
    scale = 1.0f;
    angle = 0.5f*M_PI;
    isVolumeVisualization = image->isVolume;
    type = SLICE;
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
    size = image->getSize();
    setLevel(0.5);
    setWindow(1.0);
    scale = 1.0f;
    angle = 0.5f*M_PI;
    isVolumeVisualization = image->isVolume;
    type = SLICE;
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

void Visualization::renderMIP(int imageNr, GdkPixbuf * pixBuf) {
    int xSize;
    int ySize;
    int zSize;
    switch(direction) {
        case X:
            // x direction
            xSize = this->size.y;
            ySize = this->size.z;
            zSize = this->size.x;
            break;
        case Y:
            // y direction
            xSize = this->size.x;
            ySize = this->size.z;
            zSize = this->size.y;
            break;
        case Z:
            // z direction
            xSize = this->size.x;
            ySize = this->size.y;
            zSize = this->size.z;
            break;
    }

    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    float cangle = cos(angle);
    float sangle = sin(angle);
    int xMultiple, yMultiple, zMultiple;
    switch(direction) {
        case X:
            xMultiple = this->size.x;
            yMultiple = this->size.x*this->size.y;
            zMultiple = 1;
            break;
        case Y:
            xMultiple = 1;
            yMultiple = this->size.x*this->size.y;
            zMultiple = this->size.x;
            break;
        case Z:
            xMultiple = 1;
            yMultiple = this->size.x;
            zMultiple = this->size.x*this->size.y;
            break;
    }
    int n = gdk_pixbuf_get_n_channels(pixBuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixBuf);
    // Initialize image to black
    for(int i = 0; i < n*xSize*ySize; i++)
        pixels[i] = 0;

    if(images[0]->isVectorType) {
        float3 * mip = new float3[xSize*ySize]();
        float3 * floatData = images[imageNr]->getVectorFloatData();
#pragma omp parallel for
        for(int x = 0; x < xSize; x++) {
            int nu = (x-(float)xSize/2.0f)*sangle + (float)xSize/2.0f;
        for(int y = 0; y < ySize; y++) {
            int v = y;
        for(int z = 0; z < zSize; z++) {
            int u = round((z-(float)zSize/2.0f)*cangle + nu);

            if(u >= 0 && u < xSize) {
                bool change;
                float3 newValue = maximum<float3>(mip[u+v*xSize], floatData[x*xMultiple+y*yMultiple+z*zMultiple], &change);
                if(change) {
                    // New maximum
                    mip[u+v*xSize] = newValue;
                    guchar * p = pixels + (u*n+(ySize-1-v)*rowstride);
                    p[0] = levelWindow(newValue.x,level[images[0]],window[images[0]]);
                    p[1] = levelWindow(newValue.y,level[images[0]],window[images[0]]);
                    p[2] = levelWindow(newValue.z,level[images[0]],window[images[0]]);
                }
            }
        }}}
        delete[] floatData;
        delete[] mip;
    } else {
        float * mip = new float[xSize*ySize]();
        float * floatData = images[imageNr]->getFloatData();
        #pragma omp parallel for
        for(int x = 0; x < xSize; x++) {
            int nu = (x-(float)xSize/2.0f)*sangle + (float)xSize/2.0f;
        for(int y = 0; y < ySize; y++) {
            int v = y;
        for(int z = 0; z < zSize; z++) {
            int u = round((z-(float)zSize/2.0f)*cangle + nu);

            if(u >= 0 && u < xSize) {
                bool change;
                float newValue = maximum<float>(mip[u+v*xSize], floatData[x*xMultiple+y*yMultiple+z*zMultiple], &change);
                if(change) {
                    // New maximum
                    mip[u+v*xSize] = newValue;
                    guchar * p = pixels + (u*n+(ySize-1-v)*rowstride);
                    if(images.size() == 1) {
                        p[0] = levelWindow(newValue, level[images[0]], window[images[0]]);
                        p[1] = p[0];
                        p[2] = p[0];
                    } else {
                        p[imageNr] = levelWindow(newValue, level[images[imageNr]], window[images[imageNr]]);
                    }
                }
            }
        }}}
        delete[] floatData;
        delete[] mip;
    }
}

void Visualization::renderSlice(int imageNr, GdkPixbuf * pixBuf) {
    int3 size = images[0]->getSize();
    int xSize = this->width;
    int ySize = this->height;

    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    int n =  gdk_pixbuf_get_n_channels(pixBuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixBuf);
    if(images[0]->isVectorType) {
        float3 * floatData = images[0]->getVectorFloatData();
#pragma omp parallel for
        for(int y = 0; y < ySize; y++) {
        for(int x = 0; x < xSize; x++) {
            float3 intensity;
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
            p[0] = levelWindow(intensity.x,level[images[0]],window[images[0]]);
            p[1] = levelWindow(intensity.y,level[images[0]],window[images[0]]);
            p[2] = levelWindow(intensity.z,level[images[0]],window[images[0]]);
       }}
       delete[] floatData;

    } else {
        float * floatData = images[imageNr]->getFloatData();
#pragma omp parallel for
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
}

void Visualization::renderImage(int imageNr, GdkPixbuf * pixBuf) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    int n =  gdk_pixbuf_get_n_channels(pixBuf);

    if(images[0]->isVectorType) {
        float3 * floatData = images[0]->getVectorFloatData();
#pragma omp parallel for
        for(int i = 0; i < images[0]->getTotalSize(); i++) {
            guchar * p = pixels + i * n;
            p[0] = levelWindow(floatData[i].x,level[images[0]],window[images[0]]);
            p[1] = levelWindow(floatData[i].y,level[images[0]],window[images[0]]);
            p[2] = levelWindow(floatData[i].z,level[images[0]],window[images[0]]);
        }
        delete[] floatData;
    } else {
        float * floatData = images[imageNr]->getFloatData();
#pragma omp parallel for
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
}

GdkPixbuf * Visualization::render() {
    GdkPixbuf * pixBuf;
    if(this->type == SLICE) {
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
    this->width = xSize;
    this->height = ySize;

    gtkImage = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, xSize,ySize));

    pixBuf = gtk_image_get_pixbuf((GtkImage *) gtkImage);
    for(int i = 0; i < images.size(); i++) {
        if(isVolumeVisualization) {
            renderSlice(i, pixBuf);
        } else {
            renderImage(i, pixBuf);
        }
    }
    } else if(this->type == MIP) {
        int xSize;
        int ySize;
        int zSize;
        switch(direction) {
            case X:
                // x direction
                xSize = this->size.y;
                ySize = this->size.z;
                zSize = this->size.x;
                break;
            case Y:
                // y direction
                xSize = this->size.x;
                ySize = this->size.z;
                zSize = this->size.y;
                break;
            case Z:
                // z direction
                xSize = this->size.x;
                ySize = this->size.y;
                zSize = this->size.z;
                break;
        }
        this->width = xSize;
        this->height = ySize;

        gtkImage = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
                8, xSize,ySize));
        pixBuf = gtk_image_get_pixbuf((GtkImage *) gtkImage);
        for(int i = 0; i < images.size(); i++) {
            renderMIP(i, pixBuf);
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

void SIPL::Visualization::setTitle(std::string title) {
    this->title = title;
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
	if(title == "") {
        char * title = new char[20];
        sprintf(title, "Visualization #%d", Visualization::windowCounter);
        gtk_window_set_title(GTK_WINDOW(window), title);
	} else {
	    gtk_window_set_title(GTK_WINDOW(window), this->title.c_str());
	}
	GtkWidget * toolbar = gtk_toolbar_new();
	gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_append_item (
			 GTK_TOOLBAR (toolbar), /* our toolbar */
             "Save",               /* button label */
             "Save this image",     /* this button's tooltip */
             NULL,             /* tooltip private info */
             NULL,                 /* icon widget */
             GTK_SIGNAL_FUNC(saveDialog), /* a signal */
             gtk_image_new_from_pixbuf(pixBuf));
	gtk_toolbar_append_item (
			 GTK_TOOLBAR (toolbar), /* our toolbar */
             "Close",               /* button label */
             "Close this image",     /* this button's tooltip */
             NULL,             /* tooltip private info */
             NULL,                 /* icon widget */
             GTK_SIGNAL_FUNC (signalDestroyWindow), /* a signal */
             window);
	gtk_toolbar_append_item (
			 GTK_TOOLBAR (toolbar), /* our toolbar */
             "Close program",               /* button label */
             "Close this program",     /* this button's tooltip */
             NULL,             /* tooltip private info */
             NULL,                 /* icon widget */
             GTK_SIGNAL_FUNC (quitProgram), /* a signal */
             NULL);

	gtk_window_set_default_size(
			GTK_WINDOW(window),
			size.x,
			size.y + 35
	);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(
			G_OBJECT(window),
			"destroy",
			G_CALLBACK(destroyWindow),
			NULL
	);
    g_signal_connect(
            G_OBJECT(window),
            "key_press_event",
            G_CALLBACK(Visualization::keyPressed),
            this
    );

    GtkWidget * vbox = gtk_vbox_new(FALSE, 1);

	gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar,FALSE,FALSE,0);
    scaledImage = gtk_image_new_from_pixbuf(pixBuf);
    GtkWidget *  scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport((GtkScrolledWindow *)scrolledWindow, scaledImage);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow,TRUE,TRUE,0);
	gtk_widget_show_all(window);

	gdk_threads_leave();
}
int validateSlice(int slice, slice_plane direction, int3 size) {
    if(slice < 0)
        return 0;

    switch(direction) {
        case X:
            if(slice > size.x-1)
                return size.x-1;
            break;
        case Y:
            if(slice > size.y-1)
                return size.y-1;
            break;
        case Z:
            if(slice > size.z-1)
                return size.z-1;
            break;
    }
    return slice;
}

void Visualization::keyPressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data) {
    Visualization * v = (Visualization *)user_data;
    switch(event->keyval) {
        case GDK_KEY_plus:
        case GDK_KEY_KP_Add:
            v->zoomIn();
            return;
            break;
        case GDK_KEY_minus:
        case GDK_KEY_KP_Subtract:
            v->zoomOut();
            return;
            break;

    }

    if(v->isVolumeVisualization) {
        switch(event->keyval) {
            case GDK_KEY_Up:
                v->setSlice(validateSlice(v->getSlice()+1,v->getDirection(),v->getSize()));
                break;
            case GDK_KEY_Down:
                v->setSlice(validateSlice(v->getSlice()-1,v->getDirection(),v->getSize()));
                break;
            case GDK_KEY_x:
                v->setDirection(X);
                break;
            case GDK_KEY_y:
                v->setDirection(Y);
                break;
            case GDK_KEY_z:
                v->setDirection(Z);
                break;
            case GDK_KEY_Left:
                v->setAngle(v->getAngle()-0.1f);
                break;
            case GDK_KEY_Right:
                v->setAngle(v->getAngle()+0.1f);
                break;
        }

        v->update();
    }
}

slice_plane Visualization::getDirection() const {
    return direction;
}

void Visualization::setDirection(slice_plane direction) {
    this->direction = direction;
}

int Visualization::getSlice() const {
    return slice;
}

void Visualization::setSlice(int slice) {
    this->slice = slice;
}

int3 Visualization::getSize() {
    return size;
}

void Visualization::setType(visualizationType type) {
    this->type = type;
}

void Visualization::draw() {
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(gtkImage));
    GdkPixbuf * newPixBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, scale*width, scale*height);
    gdk_pixbuf_scale(pixBuf, newPixBuf, 0, 0, scale*width, scale*height, 0, 0, scale, scale, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(scaledImage), newPixBuf);
    gtk_widget_queue_draw(scaledImage);
}

void Visualization::zoomIn() {
    // TODO: check if image is actually displayed
    scale = scale*2;
    this->draw();
}

float Visualization::getAngle() const {
    return angle;
}

void Visualization::setAngle(float angle) {
    this->angle = angle;
}

void Visualization::zoomOut() {
    // TODO: check if image is actually displayed
    scale = scale*0.5f;
    this->draw();
}

void Visualization::update() {
    GdkPixbuf * pixBuf = render();
    gtk_image_set_from_pixbuf(GTK_IMAGE(gtkImage), pixBuf);
    draw();
}
