/**
 * Simple Image Processing Library
 * Copyright Erik Smistad 2012
 * See LICENSE file for information on use 
 */
#include "Core.hpp"
#include "Visualization.hpp"

namespace SIPL {
bool init = false;
GThread * gtkThread;
int windowCount;
GMutex * windowCountMutex;
void * initGTK(void * t) {
	gdk_threads_init ();	
	gtk_init(0, (char ***) "");
	init = true;
    windowCountMutex = g_mutex_new();
	gdk_threads_enter ();
	gtk_main();
    gdk_threads_leave();
    return 0;
}

void Quit() {
    gtk_main_quit();
	g_thread_join(gtkThread);
    exit(0);
}

bool isInit() {
    return init;
}

void saveImage(BaseDataset * d, const char * filepath, const char * imageType) {
    Visualization * v = new Visualization(d);
    GdkPixbuf * pixBuf = v->render();
    delete v;
	gdk_pixbuf_save(pixBuf, filepath, imageType, NULL, NULL);
}

Visualization * displayVisualization(BaseDataset * d, float level, float window) {
    Visualization * v = new Visualization(d);
    v->setLevel(level);
    v->setWindow(window);
    v->display();
    return v;
}

Visualization * displayVolumeVisualization(BaseDataset * d, int slice, slice_plane direction, float level, float window) {
    Visualization * v = new Visualization(d);
    v->setLevel(level);
    v->setWindow(window);
    v->setSlice(slice);
    v->setDirection(direction);
    v->display();
    return v;
}

Visualization * displayMIPVisualization(BaseDataset * d, slice_plane direction, float level, float window) {
    Visualization * v = new Visualization(d);
    v->setType(MIP);
    v->setLevel(level);
    v->setWindow(window);
    v->setDirection(direction);
    v->display();
    return v;
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
int getWindowCount() {
    g_mutex_lock(windowCountMutex);
    int wc = windowCount;
    g_mutex_unlock(windowCountMutex);
    return wc;
}
int increaseWindowCount() {
    g_mutex_lock(windowCountMutex);
    windowCount++;
    int wc = windowCount;
    g_mutex_unlock(windowCountMutex);
    return wc;
}
int decreaseWindowCount() {
    g_mutex_lock(windowCountMutex);
    windowCount--;
    int wc = windowCount;
    g_mutex_unlock(windowCountMutex);
    return wc;
}
bool endReached = false;
void quit(void) {
    endReached = true;
    if(getWindowCount() == 0)
        gtk_main_quit();
	g_thread_join(gtkThread);
}

void Init() {
    windowCount = 0;
	if (!init) {
		gtkThread = g_thread_new("main", initGTK, NULL);
	}
	while(!init); // wait for the thread to be created
    atexit(quit);
}

void destroyWindow(GtkWidget * widget, gpointer window) {
	if (decreaseWindowCount() == 0 && endReached) {
		gtk_main_quit();
	}
}

void quitProgram(GtkWidget * widget, gpointer window) {
    gtk_main_quit();
    exit(0);
}

void signalDestroyWindow(GtkWidget * widget, gpointer window) {
	if (decreaseWindowCount() == 0 && endReached) {
        gtk_main_quit();
	} else {
        gtk_widget_hide(GTK_WIDGET(window));
    }
}

void saveFileSignal(GtkWidget * widget, gpointer data) {
	// Get extension to determine image type
    std::string filepath(gtk_file_selection_get_filename (GTK_FILE_SELECTION (((_saveData *)data)->fs)));
	gdk_pixbuf_save(gtk_image_get_pixbuf(
			((_saveData *)data)->image),
			filepath.c_str(),
			filepath.substr(filepath.rfind('.')+1).c_str(),
			NULL, "quality", "100", NULL);

	gtk_widget_destroy(GTK_WIDGET(((_saveData *)data)->fs));
}

void refresh(GtkWidget * widget, gpointer data) {
    Visualization * v = (Visualization *)data;
    v->update();
}

void saveDialog(GtkWidget * widget, gpointer image) {
	GtkWidget * fileSelection = gtk_file_selection_new("Save an image");

	_saveData * data = (_saveData *)malloc(sizeof(_saveData));
	data->fs = fileSelection;
	data->image = (GtkImage *)image;

	/* Connect the ok_button to file_ok_sel function */
	g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (fileSelection)->ok_button),
			      "clicked", G_CALLBACK (saveFileSignal), data);

	/* Connect the cancel_button to destroy the widget */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileSelection)->cancel_button),
		                      "clicked", G_CALLBACK (gtk_widget_destroy),
				      G_OBJECT (fileSelection));


	gtk_widget_show(fileSelection);
}

uchar color2gray(uchar * p) {
   return 0.33*(p[0]+p[1]+p[2]); 
}

void toT(bool * r, uchar * p) {
    *r = color2gray(p) > 128;
}
void toT(uchar * r, uchar * p) {
    *r = color2gray(p);
}
void toT(char * r, uchar * p) {
    *r = color2gray(p)-128;
}
void toT(ushort * r, uchar * p) {
    *r = ((float)color2gray(p)/255.0f)*65535;
}
void toT(short * r, uchar * p) {
    *r = (((float)color2gray(p)/255.0f)-0.5f)*65535;
}
void toT(uint * r, uchar * p) {
    *r = (((float)color2gray(p)/255.0f))*4294967295;
}
void toT(int * r, uchar * p) {
    *r = (((float)color2gray(p)/255.0f)-0.5f)*4294967295;
}
void toT(float * r, uchar * p) {
    *r = (float)color2gray(p)/255.0f;
}
void toT(color_uchar * c, uchar * p) {
    c->red = p[0];
    c->green = p[1];
    c->blue = p[2];
}
void toT(color_float * c, uchar * p) {
    c->red = (float)p[0]/255.0f;
    c->green = (float)p[1]/255.0f;
    c->blue = (float)p[2]/255.0f;
}
void toT(float2 * c, uchar * p) {
    c->x = (float)p[0]/255.0f;
    c->y = (float)p[1]/255.0f;
}
void toT(float3 * c, uchar * p) {
    c->x = (float)p[0]/255.0f;
    c->y = (float)p[1]/255.0f;
    c->z = (float)p[2]/255.0f;
}
template <>
void Dataset<color_uchar>::setDefaultLevelWindow() {
    this->defaultLevel = 255*0.5;
    this->defaultWindow = 255;
}
template <>
void Dataset<uchar>::setDefaultLevelWindow() {
    this->defaultLevel = 255*0.5;
    this->defaultWindow = 255;
}
template <>
void Dataset<char>::setDefaultLevelWindow() {
    this->defaultLevel = 0;
    this->defaultWindow = 255;
}
} // End namespace
