/**
 *
 * @author Erik Smistad <smistad@idi.ntnu.no>
 */

#ifndef SIPL_H_
#define SIPL_H_

#include <stdlib.h>
#include <cmath>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

namespace SIPL {

typedef bool PIXEL_BINARY; // not implemented
typedef float PIXEL_FLOAT;
typedef unsigned char PIXEL_UCHAR;
typedef unsigned short PIXEL_USHORT; // not implemented
typedef short PIXEL_SHORT; // not implemented
typedef struct PIXEL_COLOR_FLOAT { float red, blue, green;} PIXEL_COLOR_FLOAT ;
typedef struct PIXEL_COLOR_UCHAR { unsigned char red, blue, green;} PIXEL_COLOR_UCHAR ;
typedef struct PIXEL_FLOAT2 { float x,y; } PIXEL_FLOAT2; // not implemented
typedef struct PIXEL_FLOAT3 { float x,y,z; } PIXEL_FLOAT3; // not implemented

template <class T>
class Window;

template <class T>
class Image {
    public:
        Image(const char * filepath);
        Image(unsigned int width, unsigned int height);
        ~Image();
        T get(int x, int y);
        void set(int x, int y, T pixel);
        int getWidth();
        int getHeight();
        Window<T> show();
        void crop();  // not implemented
        void save(const char * filepath, const char * imageType);
        void dataToPixbuf(GtkWidget * image);
        void pixbufToData(GtkImage * image);
    private:
        T * data;
        int width, height;
};

template <class T>
class Volume {
    public:
        Volume(const char * filename); // for reading mhd files
        Volume(const char * filename, int width, int height, int depth); // for reading raw files
        Volume(int width, int height, int depth);
        ~Volume();
        T get(int x, int y, int z);
        void set(int x, int y, int z, T value);
        int getWidth();
        int getHeight();
        int getDepth();
        Window<T> show();
        void crop();  // not implemented
        void save(const char * filepath, const char * imageType);
        void dataToPixbuf(int slice, int direction);
    private:
        T * data;
        int width, height, depth;
};

template <class T>
class Window {
    public:
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Image<T> * image);
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Volume<T> * volume);
        void destroy();
        void update();
    private:
        GtkWidget * gtkWindow;
        GtkWidget * gtkImage;
        Image<T> * image;
        Volume<T> * volume;
        bool isVolume;
};


bool init = false;
pthread_t gtkThread;

void * initGTK(void * t) {
	g_thread_init(NULL);
	gdk_threads_init ();
	gdk_threads_enter ();
	gtk_init(0, (char ***) "");
	init = true;
	gtk_main();
    gdk_threads_leave();
    return 0;
}

template <>
void Image<PIXEL_UCHAR>::pixbufToData(GtkImage * image) {
	gdk_threads_enter ();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
    for(int i = 0; i < this->width*this->height; i++) {
        guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
        unsigned char * c = (unsigned char *)((pixels + i * gdk_pixbuf_get_n_channels(pixBuf)));
        this->data[i] = c[0];
    }
    gdk_threads_leave();
}

template <>
void Image<PIXEL_COLOR_UCHAR>::pixbufToData(GtkImage * image) {
	gdk_threads_enter ();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);

    for(int i = 0; i < this->width*this->height; i++) {
        guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
        unsigned char * c = (unsigned char *)((pixels + i * gdk_pixbuf_get_n_channels(pixBuf)));
        this->data[i].red = c[0];
        this->data[i].green = c[1];
        this->data[i].blue = c[2];
    }
    gdk_threads_leave();
}

template <>
void Image<PIXEL_FLOAT>::pixbufToData(GtkImage * image) {
	gdk_threads_enter ();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);

    for(int i = 0; i < this->width*this->height; i++) {
        guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
        unsigned char * c = (unsigned char *)((pixels + i * gdk_pixbuf_get_n_channels(pixBuf)));
        this->data[i] = (float)c[0]/255.0f;
    }
    gdk_threads_leave();
}


template <class T>
Image<T>::Image(const char * filename) {
	if (!init) {

		int rc = pthread_create(&gtkThread, NULL, initGTK, NULL);

	    if (rc){
	       printf("ERROR; return code from pthread_create() is %d\n", rc);
	       return;
	    }
	}

	// Load image filename
	while(!init);
	gdk_threads_enter ();
	GtkWidget * image = gtk_image_new_from_file(filename);
	gdk_threads_leave ();
	this->height = gdk_pixbuf_get_height(gtk_image_get_pixbuf((GtkImage *) image));
	this->width = gdk_pixbuf_get_width(gtk_image_get_pixbuf((GtkImage *) image));
    this->data = new T[this->height*this->width];
    this->pixbufToData((GtkImage *)image);
}
void quit(void) {
	pthread_join(gtkThread, NULL);
}
void Init() {
    atexit(quit);
}

string intToString(int inInt) {
	stringstream ss;
	string s;
	ss << inInt;
	s = ss.str();
	return s;
}

template <class T>
Image<T>::Image(unsigned int width, unsigned int height) {
    this->data = new T[width*height];
    this->width = width;
    this->height = height;
}

template <class T>
Image<T>::~Image() {
	free(this->data);
}

template <class T>
void Image<T>::save(const char * filepath, const char * imageType = "jpeg") {
    GtkImage * image = this->dataToPixbuf();
	gdk_pixbuf_save(gtk_image_get_pixbuf((GtkImage *) image), filepath, imageType,
			NULL, "quality", "100", NULL);
}

template <class T>
void Image<T>::set(int x, int y, T value) {
    this->data[x+y*this->width] = value;
}

template <class T>
T Image<T>::get(int x, int y) {
    return this->data[x+y*this->width];
}

template <class T>
Window<T>::Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Image<T> * image) {
	this->gtkWindow = gtkWindow;
    this->gtkImage = gtkImage;
    this->image = image;
    this->isVolume = false;
}

template <class T>
Window<T>::Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Volume<T> * volume) {
	this->gtkWindow = gtkWindow;
    this->gtkImage = gtkImage;
    this->volume = volume;
    this->isVolume = true;
}

template <class T>
void Window<T>::update() {
    if(this->isVolume) {
        // TODO: Update volume
    } else {
        image->dataToPixbuf(this->gtkImage);
        gtk_widget_queue_draw(this->gtkImage);
    }
}

template <class T>
void Window<T>::destroy() {
	gtk_widget_destroy(GTK_WIDGET(this->gtkWindow));
}
unsigned char windowCount = 0;
void destroyWindow(GtkWidget * widget, gpointer window) {
	windowCount--;
	if (windowCount == 0) {
		gtk_main_quit();
        exit(0);
	} else {
		gtk_widget_destroy(GTK_WIDGET(window));
	}
}

void signalDestroyWindow(GtkWidget * widget, gpointer window) {
	gtk_widget_destroy(GTK_WIDGET(window));
}
struct _saveData {
	GtkWidget * fs;
	GtkImage * image;
};

void saveFileSignal(GtkWidget * widget, gpointer data) {
	// Get extension to determine image type
	string filepath(gtk_file_selection_get_filename (GTK_FILE_SELECTION (((_saveData *)data)->fs)));
	gdk_pixbuf_save(gtk_image_get_pixbuf(
			((_saveData *)data)->image),
			filepath.c_str(),
			filepath.substr(filepath.rfind('.')+1).c_str(),
			NULL, "quality", "100", NULL);

	gtk_widget_destroy(GTK_WIDGET(((_saveData *)data)->fs));
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


template <>
void Image<PIXEL_UCHAR>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    PIXEL_UCHAR intensity = this->data[i];
    p[0] = intensity;
    p[1] = intensity;
    p[2] = intensity;
   }
   gdk_threads_leave();
}

template <>
void Image<PIXEL_COLOR_UCHAR>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    PIXEL_COLOR_UCHAR intensity = this->data[i];
    p[0] = intensity.red;
    p[1] = intensity.green;
    p[2] = intensity.blue;
   }
   gdk_threads_leave();
}

template <>
void Image<PIXEL_FLOAT>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    guchar intensity = (guchar)round(this->data[i]*255.0f);
    p[0] = intensity;
    p[1] = intensity;
    p[2] = intensity;
   }
   gdk_threads_leave();
}


template <class T>
Window<T> Image<T>::show() {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
    this->dataToPixbuf(image);
	gdk_threads_enter();


	windowCount++;
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Window<T> * winObj = new Window<T>(window,image,this);
	gtk_window_set_title(GTK_WINDOW(window),
			("Image #" + intToString(windowCount)).c_str()
	);

	GtkWidget * toolbar = gtk_toolbar_new();
	gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_append_item (
			 GTK_TOOLBAR (toolbar), /* our toolbar */
             "Save",               /* button label */
             "Save this image",     /* this button's tooltip */
             NULL,             /* tooltip private info */
             NULL,                 /* icon widget */
             GTK_SIGNAL_FUNC(saveDialog), /* a signal */
             image);
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
             GTK_SIGNAL_FUNC (gtk_main_quit), /* a signal */
             NULL);


	gtk_window_set_default_size(
			GTK_WINDOW(window),
			this->width,
			this->height + 35
	);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(
			G_OBJECT(window),
			"destroy",
			G_CALLBACK(destroyWindow),
			winObj
	);

	GtkWidget * fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (window), fixed);
	gtk_fixed_put(GTK_FIXED(fixed), toolbar, 0,0);
	gtk_fixed_put(GTK_FIXED(fixed), image, 0, 35);
	gtk_widget_show_all(window);

	gdk_threads_leave();
	return *winObj;
}

template <class T>
int Image<T>::getWidth() {
    return this->width;
}

template <class T>
int Image<T>::getHeight() {
    return this->height;
}

} // End SIPL namespace
#endif /* SIPL_H_ */
