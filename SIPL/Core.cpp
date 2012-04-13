#include "Core.h"
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
namespace SIPL {
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

bool init = false;
pthread_t gtkThread;

void * initGTK(void * t)
{
	g_thread_init(NULL);
	gdk_threads_init ();
	gdk_threads_enter ();
	gtk_init(0, (char ***) "");
	init = true;
	gtk_main();
    gdk_threads_leave();
}

Image::Image(const char * filename) {
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
	image = gtk_image_new_from_file(filename);
	gdk_threads_leave ();
}

Image::Image(unsigned int width, unsigned int height) {
	while(!init);
	image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
}

Image::~Image() {
	//free(image);
}

void Image::save(const char * filepath, const char * imageType = "jpeg") {
	gdk_pixbuf_save(gtk_image_get_pixbuf((GtkImage *) image), filepath, imageType,
			NULL, "quality", "100", NULL);
}

Pixel::Pixel(pixel p, Image *image, int x, int y) {
	this->red = p.red;
	this->green = p.green;
	this->blue = p.blue;
	this->x = x;
	this->y = y;
	this->image = image;
}

void Pixel::set(unsigned char intensity) {
	gdk_threads_enter();
	GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image->image);
	guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
	guchar * p = pixels + x * gdk_pixbuf_get_n_channels(pixBuf) + y
			* gdk_pixbuf_get_rowstride(pixBuf);
	p[0] = intensity;
	p[1] = intensity;
	p[2] = intensity;
	gdk_threads_leave ();
}

void Pixel::set(unsigned char red, unsigned char green, unsigned char blue) {
	gdk_threads_enter();
	GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image->image);
	guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
	guchar * p = pixels + x * gdk_pixbuf_get_n_channels(pixBuf) + y
			* gdk_pixbuf_get_rowstride(pixBuf);
	p[0] = red;
	p[1] = green;
	p[2] = blue;
	gdk_threads_leave ();
}

void Pixel::set(pixel p) {
}

void Pixel::set(Pixel pix) {
}

unsigned char * Pixel::get() {
	gdk_threads_enter();
	GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image->image);
	guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    unsigned char * c = (unsigned char *)((pixels + x * gdk_pixbuf_get_n_channels(pixBuf)
			+ y * gdk_pixbuf_get_rowstride(pixBuf)));
	gdk_threads_leave ();
	return c;

}

Pixel Image::getPixel(int x, int y) {
	gdk_threads_enter();
	GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
	guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
	guchar * p = pixels + x * gdk_pixbuf_get_n_channels(pixBuf) + y
			* gdk_pixbuf_get_rowstride(pixBuf);
	pixel pi;
	pi.red = p[0];
	pi.blue = p[1];
	pi.green = p[2];
	gdk_threads_leave();
	return Pixel(pi, this, x, y);
}

Window::Window(GtkWidget * gtkWindow) {
	this->gtkWindow = gtkWindow;
}

void Window::destroy() {
	gtk_widget_destroy(GTK_WIDGET(this->gtkWindow));
}
unsigned char windowCount = 0;
void destroyWindow(GtkWidget * widget, gpointer window) {
	windowCount--;
	if (windowCount == 0) {
		gtk_main_quit();
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

Window Image::show() {
	gdk_threads_enter();
	windowCount++;
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Window * winObj = new Window(window);
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
	                             this->image);
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
			this->getWidth(),
			this->getHeight() + 35
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
int Image::getWidth() {
	return gdk_pixbuf_get_width(gtk_image_get_pixbuf((GtkImage *) image));
}

int Image::getHeight() {
	return gdk_pixbuf_get_height(gtk_image_get_pixbuf((GtkImage *) image));
}

void quit(void) {
	pthread_join(gtkThread, NULL);
}

void Window::update() {
    gtk_widget_queue_draw (gtkWindow);
}
}
