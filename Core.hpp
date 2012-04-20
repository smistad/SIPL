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
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <pthread.h>


namespace SIPL {
typedef unsigned char uchar;
typedef unsigned short ushort; // not implemented
typedef unsigned int uint; // not implemented
typedef struct color_float { float red, blue, green;} color_float ; // not implemented
typedef struct color_uchar { unsigned char red, blue, green;} color_uchar ;
typedef struct float2 { float x,y; } float2; // not implemented
typedef struct float3 { float x,y,z; } float3; // not implemented
typedef struct int2 { int x,y; } int2;
typedef struct int3 { int x,y,z; } int3;
enum slice_plane {X,Y,Z};

template <class T>
class Window;

template <class T>
class Image {
    public:
        Image(const char * filepath);
        Image(unsigned int width, unsigned int height);
        ~Image();
        T get(int x, int y);
        T * getData();
        void setData(T *);
        void set(int x, int y, T pixel);
        int getWidth();
        int getHeight();
        Window<T> show();
        Window<T> show(float level, float window);
        void crop();  // not implemented
        void save(const char * filepath, const char * imageType);
        void dataToPixbuf(GtkWidget * image);
        void dataToPixbuf(GtkWidget * image, float level, float wdinow);
        void pixbufToData(GtkImage * image);
    private:
        T * data;
        int width, height;
        Window<T> setupGUI(GtkWidget * image);
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
        T * getData();
        void setData(T *);
        int getWidth();
        int getHeight();
        int getDepth();
        Window<T> show();
        Window<T> show(int slice, slice_plane direction);
        Window<T> show(int slice, slice_plane direction, float level, float window);
        void crop();  // not implemented
        void save(const char * filepath);
        void saveSlice(int slice, slice_plane direction, const char * filepath, const char * imageType);
        void dataToPixbuf(GtkWidget * image, int slice, slice_plane direction);
    private:
        T * data;
        int width, height, depth;
        Window<T> setupGUI(GtkWidget * image);
};

template <class T>
class Window {
    public:
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Image<T> * image);
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Volume<T> * volume);
        void destroy();
        void update();
        void hide();
    private:
        GtkWidget * gtkWindow;
        GtkWidget * gtkImage;
        Image<T> * image;
        Volume<T> * volume;
        bool isVolume;
};




/* --- Spesialized pixbufToData methods --- */
template <>
void Image<uchar>::pixbufToData(GtkImage * image) {
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
void Image<color_uchar>::pixbufToData(GtkImage * image) {
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
void Image<float>::pixbufToData(GtkImage * image) {
	gdk_threads_enter ();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);

    for(int i = 0; i < this->width*this->height; i++) {
        guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
        unsigned char * c = (unsigned char *)((pixels + i * gdk_pixbuf_get_n_channels(pixBuf)));
        this->data[i] = (float)c[0]/255.0f;
    }
    gdk_threads_leave();
}

/* --- Spesialized dataToPixbuf methods --- */

template <>
void Image<uchar>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    uchar intensity = this->data[i];
    p[0] = intensity;
    p[1] = intensity;
    p[2] = intensity;
   }
   gdk_threads_leave();
}

template <>
void Volume<uchar>::dataToPixbuf(GtkWidget * image, int slice, slice_plane direction) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
    int xSize;
    int ySize;
    switch(direction) {
        case X:
            // x direction
            xSize = this->height;
            ySize = this->depth;
            break;
        case Y:
            // y direction
            xSize = this->width;
            ySize = this->depth;
            break;
        case Z:
            // z direction
            xSize = this->width;
            ySize = this->height;
            break;
    }
            
            
   for(int x = 0; x < xSize; x++) {
   for(int y = 0; y < ySize; y++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    int i = x + y *xSize;
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    uchar intensity;
    switch(direction) {
        case X:
            intensity = this->data[slice + x*this->width + y*this->width*this->height];
            break;
        case Y:
            intensity = this->data[x + slice*this->width + y*this->width*this->height];
            break;
        case Z:
            intensity = this->data[x + y*this->width + slice*this->width*this->height];
            break;
    }

    p[0] = intensity;
    p[1] = intensity;
    p[2] = intensity;
   }}
   gdk_threads_leave();
}



template <>
void Image<color_uchar>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    color_uchar intensity = this->data[i];
    p[0] = intensity.red;
    p[1] = intensity.green;
    p[2] = intensity.blue;
   }
   gdk_threads_leave();
}

template <>
void Image<float>::dataToPixbuf(GtkWidget * image) {
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

template <>
void Image<float>::dataToPixbuf(GtkWidget * image, float level, float window) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    float v = this->data[i];
    if(v < level-window*0.5f) {
        v = 0.0f;
    } else if(v > level+window*0.5f) {
        v = 1.0f;
    } else {
        v = (v-(level-window*0.5f)) / window;
    }
    guchar intensity = (guchar)round(v*255.0f);
    p[0] = intensity;
    p[1] = intensity;
    p[2] = intensity;
   }
   gdk_threads_leave();
}

/* --- Constructors & destructors --- */

template <class T>
Image<T>::Image(const char * filename) {
	gdk_threads_enter ();
	GtkWidget * image = gtk_image_new_from_file(filename);
	gdk_threads_leave ();
	this->height = gdk_pixbuf_get_height(gtk_image_get_pixbuf((GtkImage *) image));
	this->width = gdk_pixbuf_get_width(gtk_image_get_pixbuf((GtkImage *) image));
    this->data = new T[this->height*this->width];
    this->pixbufToData((GtkImage *)image);
}

template <class T>
Volume<T>::Volume(const char * filename, int width, int height, int depth) {
    // Read raw file
    this->data = new T[width*height*depth];
    FILE * file = fopen(filename, "rb");
    if(file == NULL) {
        std::cout << "File " << filename << " not found" << std::endl;
        exit(0);
    }
    fread(this->data, sizeof(T), width*height*depth, file);
    fclose(file);
    this->width = width;
    this->height = height;
    this->depth = depth;
}

template <class T>
Volume<T>::Volume(const char * filename) {
    // Current limits of this method: T and the data type to be read has to be the same. Does not handle space after strings

    // Read mhd file
    std::fstream mhdFile;
    mhdFile.open(filename, std::fstream::in);
    std::string line;
    std::string rawFilename;
    bool sizeFound = false, 
         rawFilenameFound = false, 
         typeFound = false, 
         dimensionsFound = false;
    bool isSigned;
    int readTypeSize;
    do{
        std::getline(mhdFile, line);
        if(line.substr(0, 7) == "DimSize") {
            std::string sizeString = line.substr(7+3);
            std::string sizeX = sizeString.substr(0,sizeString.find(" "));
            sizeString = sizeString.substr(sizeString.find(" ")+1);
            std::string sizeY = sizeString.substr(0,sizeString.find(" "));
            sizeString = sizeString.substr(sizeString.find(" ")+1);
            std::string sizeZ = sizeString.substr(0,sizeString.find(" "));

            this->width = atoi(sizeX.c_str());
            this->height = atoi(sizeY.c_str());
            this->depth = atoi(sizeZ.c_str());

            sizeFound = true;
        } else if(line.substr(0, 15) == "ElementDataFile") {
            rawFilename = line.substr(15+3);
            rawFilenameFound = true;
        } else if(line.substr(0, 11) == "ElementType") {
            typeFound = true;
            std::string typeName = line.substr(11+3);
            std::cout << typeName << std::endl;
            if(typeName == "MET_SHORT") {
                readTypeSize = sizeof(short);
                isSigned = true;
            } else if(typeName == "MET_USHORT") {
                readTypeSize = sizeof(short);
                isSigned = false;
            } else if(typeName == "MET_CHAR") {
                readTypeSize = sizeof(char);
                isSigned = true;
            } else if(typeName == "MET_UCHAR") {
                readTypeSize = sizeof(char);
                isSigned = false;
            } else if(typeName == "MET_INT") {
                readTypeSize = sizeof(int);
                isSigned = true;
            } else if(typeName == "MET_UINT") {
                readTypeSize = sizeof(int);
                isSigned = false;
            } else if(typeName == "MET_FLOAT") {
                readTypeSize = sizeof(float);
                isSigned = true;
            } else {
                std::cout << "Error: Trying to read volume of unsupported data type" << std::endl;
                exit(-1);
            }
        } else if(line.substr(0, 5) == "NDims") {
            if(line.substr(5+3, 1) == "3") 
                dimensionsFound = true;
        }
    } while(!mhdFile.eof());

    mhdFile.close();
    if(!sizeFound || !rawFilenameFound || !typeFound || !dimensionsFound) {
        std::cout << "Error reading the mhd file" << std::endl;
        exit(-1);
    }

    if(readTypeSize != sizeof(T)) {
        std::cout << readTypeSize << std::endl;
        std::cout << "Error: mismatch between datatype to read and that of Volume object (size). Conversion not supported yet." << std::endl;
        exit(-1);
    }

    if(((T)(-1) > 0 && isSigned) || ((T)(-1) < 0 && !isSigned)) {
        std::cout << "Error: mismatch between datatype to read and that of Volume object (sign). Conversion not supported yet." << std::endl;
        exit(-1);
    }

    // Read raw file
    this->data = new T[width*height*depth];
    FILE * file = fopen(rawFilename.c_str(), "rb");
    if(file == NULL) {
        std::cout << "File " << rawFilename << " not found" << std::endl;
        exit(0);
    }
    fread(this->data, sizeof(T), width*height*depth, file);
    fclose(file);
}

template <class T>
Volume<T>::Volume(int width, int height, int depth) {
    this->data = new T[width*height*depth];
    this->width = width;
    this->height = height;
    this->depth = depth;
}

template <class T>
Image<T>::Image(unsigned int width, unsigned int height) {
    this->data = new T[width*height];
    this->width = width;
    this->height = height;
}

template <class T>
Image<T>::~Image() {
	delete[] this->data;
}


template <class T>
Volume<T>::~Volume() {
    delete[] this->data;
}

/* Init and Quit stuff */

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

void quit(void) {
	pthread_join(gtkThread, NULL);
}

void Init() {
	if (!init) {

		int rc = pthread_create(&gtkThread, NULL, initGTK, NULL);

	    if (rc){
	       printf("ERROR; return code from pthread_create() is %d\n", rc);
           exit(-1);
	    }
	}

	while(!init); // wait for the thread to created
    atexit(quit);
}

std::string intToString(int inInt) {
    std::stringstream ss;
    std::string s;
	ss << inInt;
	s = ss.str();
	return s;
}

template <class T>
void Image<T>::save(const char * filepath, const char * imageType = "jpeg") {
    GtkImage * image = this->dataToPixbuf();
	gdk_pixbuf_save(gtk_image_get_pixbuf((GtkImage *) image), filepath, imageType,
			NULL, "quality", "100", NULL);
}

template <class T>
void Volume<T>::save(const char * filepath) {
    // This might not work for the defined struct types?
    FILE * file = fopen(filepath, "wb");
    if(file == NULL) {
        std::cout << "Could not write RAW file to " << filepath << std::endl;
        exit(-1);
    }

    fwrite(this->data, sizeof(T), this->width*this->height*this->depth, file);
    fclose(file);
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
	}
}

void quitProgram(GtkWidget * widget, gpointer window) {
    gtk_main_quit();
    exit(0);
}

void signalDestroyWindow(GtkWidget * widget, gpointer window) {
	windowCount--;
	if (windowCount == 0) {
		gtk_main_quit();
        exit(0);
	} else {
        gtk_widget_hide(GTK_WIDGET(window));
    }
}
struct _saveData {
	GtkWidget * fs;
	GtkImage * image;
};

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


template <class T>
Window<T> Image<T>::setupGUI(GtkWidget * image) {

	gdk_threads_enter();
	windowCount++;
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Window<T> winObj = Window<T>(window,image,this);
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
             GTK_SIGNAL_FUNC (quitProgram), /* a signal */
             NULL);


	gtk_window_set_default_size(
			GTK_WINDOW(window),
			width,
			height + 35
	);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(
			G_OBJECT(window),
			"destroy",
			G_CALLBACK(destroyWindow),
			&winObj
	);

	GtkWidget * fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (window), fixed);
	gtk_fixed_put(GTK_FIXED(fixed), toolbar, 0,0);
	gtk_fixed_put(GTK_FIXED(fixed), image, 0, 35);
	gtk_widget_show_all(window);

	gdk_threads_leave();
    return winObj;

}

template <class T>
Window<T> Volume<T>::setupGUI(GtkWidget * image) {

	gdk_threads_enter();
	windowCount++;
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	Window<T> winObj = Window<T>(window,image,this);
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
             GTK_SIGNAL_FUNC (quitProgram), /* a signal */
             NULL);


	gtk_window_set_default_size(
			GTK_WINDOW(window),
			width,
			height + 35
	);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(
			G_OBJECT(window),
			"destroy",
			G_CALLBACK(destroyWindow),
			&winObj
	);

	GtkWidget * fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (window), fixed);
	gtk_fixed_put(GTK_FIXED(fixed), toolbar, 0,0);
	gtk_fixed_put(GTK_FIXED(fixed), image, 0, 35);
	gtk_widget_show_all(window);

	gdk_threads_leave();
    return winObj;

}


template <class T>
Window<T> Image<T>::show() {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
    this->dataToPixbuf(image);
	return setupGUI(image);
}

template <class T>
Window<T> Image<T>::show(float level, float window) {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
    this->dataToPixbuf(image, level, window);
	return setupGUI(image);
}

template <class T>
Window<T> Volume<T>::show(){
    int slice = this->height/2;
    slice_plane direction = Z;
    int displayWidth = this->width;
    int displayHeight = this->height;
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, displayWidth, displayHeight));
    this->dataToPixbuf(image, slice, direction);
    return setupGUI(image);
}


template <class T>
Window<T> Volume<T>::show(int slice, slice_plane direction) {
    int displayWidth;
    int displayHeight;
    switch(direction) {
        case X:
            displayWidth = this->height;
            displayHeight = this->depth;
            break;
        case Y:
            displayWidth = this->width;
            displayHeight = this->depth;
            break;
        case Z:
            displayWidth = this->width;
            displayHeight = this->height;
            break;
    }
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, displayWidth, displayHeight));
    this->dataToPixbuf(image, slice, direction);
    return setupGUI(image);
}

template <class T>
int Image<T>::getWidth() {
    return this->width;
}

template <class T>
int Image<T>::getHeight() {
    return this->height;
}

template <class T>
int Volume<T>::getWidth() {
    return this->width;
}

template <class T>
int Volume<T>::getHeight() {
    return this->height;
}

template <class T>
int Volume<T>::getDepth() {
    return this->depth;
}

template <class T>
void Image<T>::setData(T * data) {
    this->data = data;
}

template <class T>
T * Image<T>::getData() {
    return this->data;
}

template <class T>
void Volume<T>::setData(T * data) {
    this->data = data;
}

template <class T>
T * Volume<T>::getData() {
    return this->data;
}

} // End SIPL namespace
#endif /* SIPL_H_ */
