/**
 * Simple Image Processing Library
 * Copyright Erik Smistad 2012
 * See LICENSE file for information on use 
 */

#ifndef SIPL_H_
#define SIPL_H_

#include <math.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

namespace SIPL {
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint; 
typedef struct color_float { float red, blue, green;} color_float ; 
typedef struct color_uchar { unsigned char red, blue, green;} color_uchar ;
typedef struct float2 { float x,y; } float2; 
typedef struct float3 { float x,y,z; } float3; 
typedef struct int2 { int x,y; } int2; // not for images/volumes
typedef struct int3 { int x,y,z; } int3; // not for images/volumes
enum slice_plane {X,Y,Z};

void Init();
void End();
void Quit();

void destroyWindow(GtkWidget * widget, gpointer window) ;
void quitProgram(GtkWidget * widget, gpointer window) ;
void signalDestroyWindow(GtkWidget * widget, gpointer window) ;
void saveFileSignal(GtkWidget * widget, gpointer data) ;
void saveDialog(GtkWidget * widget, gpointer image) ;
int increaseWindowCount() ;

template <class T>
class Window;

template <class T>
class Image {
    public:
        Image(const char * filepath);
        Image(unsigned int width, unsigned int height);
        template <class U>
        Image(Image<U> &otherImage);
        ~Image();
        T get(int x, int y) const;
        T get(int i) const;
        T * getData();
        void setData(T *);
        void set(int x, int y, T pixel);
        void set(int i, T pixel);
        int getWidth() const;
        int getHeight() const;
        int2 getSize() const;
        Window<T> * show();
        Window<T> * show(float level, float window);
        void save(const char * filepath, const char * imageType);
        void dataToPixbuf(GtkWidget * image);
        void dataToPixbuf(GtkWidget * image, float level, float wdinow);
        void pixbufToData(GtkImage * image);
        template <class U>
        Image<T> & operator=(const Image<U> &otherImage);
		static gboolean setupGUI(gpointer data);
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
        template <class U>
        Volume(Volume<U> &otherVolume);
        ~Volume();
        T get(int x, int y, int z) const;
        T get(int i) const;
        void set(int x, int y, int z, T value);
        void set(int i, T pixel);
        T * getData();
        void setData(T *);
        int getWidth() const;
        int getHeight() const;
        int getDepth() const;
        int3 getSize() const;
        Window<T> * show();
        Window<T> * show(int slice, slice_plane direction);
        Window<T> * show(int slice, slice_plane direction, float level, float window);
        Window<T> * show(float level, float window);
        void save(const char * filepath);
        void saveSlice(int slice, slice_plane direction, const char * filepath, const char * imageType);
        void dataToPixbuf(GtkWidget * image, int slice, slice_plane direction);
        void dataToPixbuf(GtkWidget * image, int slice, slice_plane direction, float level, float window);
		static gboolean setupGUI(gpointer data);
        template <class U>
        Volume<T> & operator=(const Volume<U> &otherVolume);
    private:
        T * data;
        int width, height, depth;
};

template <class T>
class Window {
    public:
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Image<T> * image);
        Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Volume<T> * volume);
        void key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data) ;
        static void wrapper_key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data);
        void destroy();
        void update();
        void hide();
        int currentSlice;
        slice_plane currentDirection;
        float level, window;
        ~Window();
    private:
		GtkWidget * gtkWindow;
		GtkWidget * gtkImage;
		Image<T> * image;
        Volume<T> * volume;
        bool isVolume;
        friend class Image<T>;
        friend class Volume<T>;
};

template <class T>
Window<T>::~Window() {
}

/* --- Spesialized conversion functions --- */
void toGuchar(bool value, guchar * pixel) ;
void toGuchar(uchar value, guchar * pixel) ;
void toGuchar(char value, guchar * pixel) ;
void toGuchar(ushort value, guchar * pixel) ;
void toGuchar(short value, guchar * pixel) ;
void toGuchar(uint value, guchar * pixel) ;
void toGuchar(int value, guchar * pixel) ;
void toGuchar(float value, guchar * pixel) ;
void toGuchar(color_uchar value, guchar * pixel) ;
void toGuchar(color_float value, guchar * pixel) ;
void toGuchar(float2 value, guchar * pixel) ;
void toGuchar(float3 value, guchar * pixel) ;

// Conversion from bool
void convertImageType(bool *, bool);
void convertImageType(uchar *, bool);
void convertImageType(char *, bool);
void convertImageType(ushort *, bool);
void convertImageType(short *, bool);
void convertImageType(uint *, bool);
void convertImageType(int *, bool);
void convertImageType(float *, bool);
void convertImageType(color_uchar *, bool);
void convertImageType(color_float *, bool);
void convertImageType(float2 *, bool);
void convertImageType(float3 *, bool);

// Conversion from uchar
void convertImageType(bool *, uchar);
void convertImageType(uchar *, uchar);
void convertImageType(char *, uchar);
void convertImageType(ushort *, uchar);
void convertImageType(short *, uchar);
void convertImageType(uint *, uchar);
void convertImageType(int *, uchar);
void convertImageType(float *, uchar);
void convertImageType(color_uchar *, uchar);
void convertImageType(color_float *, uchar);
void convertImageType(float2 *, uchar);
void convertImageType(float3 *, uchar);

// Conversion from char
void convertImageType(bool *, char);
void convertImageType(uchar *, char);
void convertImageType(char *, char);
void convertImageType(ushort *, char);
void convertImageType(short *, char);
void convertImageType(uint *, char);
void convertImageType(int *, char);
void convertImageType(float *, char);
void convertImageType(color_uchar *, char);
void convertImageType(color_float *, char);
void convertImageType(float2 *, char);
void convertImageType(float3 *, char);

// Conversion from ushort
void convertImageType(bool *, ushort);
void convertImageType(uchar *, ushort);
void convertImageType(char *, ushort);
void convertImageType(ushort *, ushort);
void convertImageType(short *, ushort);
void convertImageType(uint *, ushort);
void convertImageType(int *, ushort);
void convertImageType(float *, ushort);
void convertImageType(color_uchar *, ushort);
void convertImageType(color_float *, ushort);
void convertImageType(float2 *, ushort);
void convertImageType(float3 *, ushort);

// Conversion from short
void convertImageType(bool *, short);
void convertImageType(uchar *, short);
void convertImageType(char *, short);
void convertImageType(ushort *, short);
void convertImageType(short *, short);
void convertImageType(uint *, short);
void convertImageType(int *, short);
void convertImageType(float *, short);
void convertImageType(color_uchar *, short);
void convertImageType(color_float *, short);
void convertImageType(float2 *, short);
void convertImageType(float3 *, short);

// Conversion from float
void convertImageType(bool *, float);
void convertImageType(uchar *, float);
void convertImageType(char *, float);
void convertImageType(ushort *, float);
void convertImageType(short *, float);
void convertImageType(uint *, float);
void convertImageType(int *, float);
void convertImageType(float *, float);
void convertImageType(color_uchar *, float);
void convertImageType(color_float *, float);
void convertImageType(float2 *, float);
void convertImageType(float3 *, float);

// Conversion from color_uchar
void convertImageType(bool *, color_uchar);
void convertImageType(uchar *, color_uchar);
void convertImageType(char *, color_uchar);
void convertImageType(ushort *, color_uchar);
void convertImageType(short *, color_uchar);
void convertImageType(uint *, color_uchar);
void convertImageType(int *, color_uchar);
void convertImageType(float *, color_uchar);
void convertImageType(color_uchar *, color_uchar);
void convertImageType(color_float *, color_uchar);
void convertImageType(float2 *, color_uchar);
void convertImageType(float3 *, color_uchar);

// Conversion from color_float
void convertImageType(bool *, color_float);
void convertImageType(uchar *, color_float);
void convertImageType(char *, color_float);
void convertImageType(ushort *, color_float);
void convertImageType(short *, color_float);
void convertImageType(uint *, color_float);
void convertImageType(int *, color_float);
void convertImageType(float *, color_float);
void convertImageType(color_uchar *, color_float);
void convertImageType(color_float *, color_float);
void convertImageType(float2 *, color_float);
void convertImageType(float3 *, color_float);

// Conversion from float2
void convertImageType(bool *, float2);
void convertImageType(uchar *, float2);
void convertImageType(char *, float2);
void convertImageType(ushort *, float2);
void convertImageType(short *, float2);
void convertImageType(uint *, float2);
void convertImageType(int *, float2);
void convertImageType(float *, float2);
void convertImageType(color_uchar *, float2);
void convertImageType(color_float *, float2);
void convertImageType(float2 *, float2);
void convertImageType(float3 *, float2);

// Conversion from float3
void convertImageType(bool *, float3);
void convertImageType(uchar *, float3);
void convertImageType(char *, float3);
void convertImageType(ushort *, float3);
void convertImageType(short *, float3);
void convertImageType(uint *, float3);
void convertImageType(int *, float3);
void convertImageType(float *, float3);
void convertImageType(color_uchar *, float3);
void convertImageType(color_float *, float3);
void convertImageType(float2 *, float3);
void convertImageType(float3 *, float3);

/* --- Spesialized level/window --- */
void toGuchar(uchar value, guchar * pixel, float level, float window) ;
void toGuchar(char value, guchar * pixel, float level, float window) ;
void toGuchar(ushort value, guchar * pixel, float level, float window) ;
void toGuchar(short value, guchar * pixel, float level, float window) ;
void toGuchar(uint value, guchar * pixel, float level, float window) ;
void toGuchar(int value, guchar * pixel, float level, float window) ;
void toGuchar(float value, guchar * pixel, float level, float window) ;
void toGuchar(color_float value, guchar * pixel, float level, float window) ;
void toGuchar(float2 value, guchar * pixel, float level, float window) ;
void toGuchar(float3 value, guchar * pixel, float level, float window) ;
inline double round( double d ) {
    return floor( d + 0.5 );
}
template <class T>
uchar levelWindow(T value, float level, float window) {
    float result;
    if(value < level-window*0.5f) {
        result = 0.0f;
    } else if(value > level+window*0.5f) {
        result = 1.0f;
    } else {
        result = (float)(value-(level-window*0.5f)) / window;
    }
    result = round(result*255);
    return result;
}

/* --- Spesialized toT functions --- */

void toT(bool * r, uchar * p) ;
void toT(uchar * r, uchar * p) ;
void toT(char * r, uchar * p) ;
void toT(ushort * r, uchar * p) ;
void toT(short * r, uchar * p) ;
void toT(uint * r, uchar * p) ;
void toT(int * r, uchar * p) ;
void toT(float * r, uchar * p) ;
void toT(color_uchar * r, uchar * p) ;
void toT(color_float * r, uchar * p) ;
void toT(float2 * r, uchar * p) ;
void toT(float3 * r, uchar * p) ;

/* --- Spesialized pixbufToData methods --- */
template <class T>
void Image<T>::pixbufToData(GtkImage * image) {
	gdk_threads_enter ();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
    for(int i = 0; i < this->width*this->height; i++) {
        guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
        unsigned char * c = (unsigned char *)((pixels + i * gdk_pixbuf_get_n_channels(pixBuf)));
        toT(&this->data[i], c);
    }
    gdk_threads_leave();
}

/* --- Spesialized dataToPixbuf methods --- */
template <class T>
void Image<T>::dataToPixbuf(GtkWidget * image) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    toGuchar(this->data[i], p);
   }
   gdk_threads_leave();
}

template <class T>
void Image<T>::dataToPixbuf(GtkWidget * image, float level, float window) {
    gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
   for(int i = 0; i < this->width*this->height; i++) {
    guchar * pixels = gdk_pixbuf_get_pixels(pixBuf);
    guchar * p = pixels + i * gdk_pixbuf_get_n_channels(pixBuf);
    toGuchar(this->data[i], p, level, window);
   }
   gdk_threads_leave();
}

int validateSlice(int slice, slice_plane direction, int3 size);

template <class T>
void Volume<T>::dataToPixbuf(GtkWidget * image, int slice, slice_plane direction) {
    //gdk_threads_enter();
    GdkPixbuf * pixBuf = gtk_image_get_pixbuf((GtkImage *) image);
    validateSlice(slice, direction, getSize());
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
    T intensity;
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

    toGuchar(intensity, p);
   }}
   //gdk_threads_leave();
}

template <class T>
void Volume<T>::dataToPixbuf(GtkWidget * image, int slice, slice_plane direction, float level, float window) {
    //gdk_threads_enter();
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
    T intensity;
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

    toGuchar(intensity, p, level, window);
   }}
   //gdk_threads_leave();
}


/* --- Constructors & destructors --- */
template <class T>
Image<T>::Image(const char * filename) {
    // Check if file exists
    FILE * file = fopen(filename, "r");
    if(file == NULL) {
        std::cout << "Error: " << filename << " not found" << std::endl;
        Quit();
    }
    fclose(file);
	gdk_threads_enter ();
	GtkWidget * image = gtk_image_new_from_file(filename);
	gdk_threads_leave ();
	this->height = gdk_pixbuf_get_height(gtk_image_get_pixbuf((GtkImage *) image));
	this->width = gdk_pixbuf_get_width(gtk_image_get_pixbuf((GtkImage *) image));
    this->data = new T[this->height*this->width];
    this->pixbufToData((GtkImage *)image);
}

template <class T> 
template <class U>
Image<T>::Image(Image<U> &otherImage) {
    this->width = otherImage.getWidth();
    this->height = otherImage.getHeight();
    this->data = new T[this->height*this->width];

    // Convert image with type U to this with type T
    for(int i = 0; i < this->width*this->height; i++) {
        T value;
        convertImageType(&value, otherImage.get(i));
        this->data[i] = value;
    }

}

template <class T> 
template <class U>
Image<T>& Image<T>::operator=(const Image<U> &otherImage) {
    if(this->width != otherImage.getWidth() || this->height != otherImage.getHeight()) {
        std::cout << "Error: image size mismatch in assignment" << std::endl;
        Quit();
    }
    
    // Convert image with type U to this with type T
    for(int i = 0; i < this->width*this->height; i++) {
        T value;
        convertImageType(&value, otherImage.get(i));
        this->data[i] = value;
    }

    return *this;
}

template <class T> 
template <class U>
Volume<T>::Volume(Volume<U> &otherImage) {
    this->width = otherImage.getWidth();
    this->height = otherImage.getHeight();
    this->depth = otherImage.getDepth();
    this->data = new T[this->height*this->width*this->depth];

    // Convert image with type U to this with type T
    for(int i = 0; i < this->width*this->height*this->depth; i++) {
        T value;
        convertImageType(&value, otherImage.get(i));
        this->data[i] = value;
    }
}

template <class T> 
template <class U>
Volume<T>& Volume<T>::operator=(const Volume<U> &otherImage) {
    if(this->width != otherImage.getWidth() || 
        this->height != otherImage.getHeight() ||
        this->depth != otherImage.getDepth()) {
        std::cout << "Error: volume size mismatch in assignment" << std::endl;
        Quit();
    }
    
    // Convert image with type U to this with type T
    for(int i = 0; i < this->width*this->height*this->depth; i++) {
        T value;
        convertImageType(&value, otherImage.get(i));
        this->data[i] = value;
    }

    return *this;
}


template <class T>
Volume<T>::Volume(const char * filename, int width, int height, int depth) {
    // Read raw file
    this->data = new T[width*height*depth];
    FILE * file = fopen(filename, "rb");
    if(file == NULL) {
        std::cout << "Error: File " << filename << " not found" << std::endl;
        Quit();
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
                Quit();
            }
        } else if(line.substr(0, 5) == "NDims") {
            if(line.substr(5+3, 1) == "3") 
                dimensionsFound = true;
        }
    } while(!mhdFile.eof());

    mhdFile.close();
    if(!sizeFound || !rawFilenameFound || !typeFound || !dimensionsFound) {
        std::cout << "Error reading the mhd file" << std::endl;
        Quit();
    }

    if(readTypeSize != sizeof(T)) {
        std::cout << readTypeSize << std::endl;
        std::cout << "Error: mismatch between datatype to read and that of Volume object (size). Conversion not supported yet." << std::endl;
        Quit();
    }

    if(((T)(-1) > 0 && isSigned) || ((T)(-1) < 0 && !isSigned)) {
        std::cout << "Error: mismatch between datatype to read and that of Volume object (sign). Conversion not supported yet." << std::endl;
        Quit();
    }

    // Read raw file
    this->data = new T[width*height*depth];
    FILE * file = fopen(rawFilename.c_str(), "rb");
    if(file == NULL) {
        std::cout << "File " << rawFilename << " not found" << std::endl;
        Quit();
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

static std::string intToString(int inInt) {
    std::stringstream ss;
    std::string s;
	ss << inInt;
	s = ss.str();
	return s;
}

template <class T>
void Image<T>::save(const char * filepath, const char * imageType = "jpeg") {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
	this->dataToPixbuf(image);
	gdk_pixbuf_save(gtk_image_get_pixbuf((GtkImage *) image), filepath, imageType,
			NULL, NULL);
}

template <class T>
void Volume<T>::save(const char * filepath) {
    // This might not work for the defined struct types?
    FILE * file = fopen(filepath, "wb");
    if(file == NULL) {
        std::cout << "Could not write RAW file to " << filepath << std::endl;
        Quit();
    }

    fwrite(this->data, sizeof(T), this->width*this->height*this->depth, file);
    fclose(file);
}

template <class T>
void Image<T>::set(int x, int y, T value) {
    this->data[x+y*this->width] = value;
}

template <class T>
void Image<T>::set(int i, T value) {
    this->data[i] = value;
}

template <class T>
void Volume<T>::set(int i, T value) {
    this->data[i] = value;
}



template <class T>
T Image<T>::get(int x, int y) const {
    return this->data[x+y*this->width];
}

template <class T>
T Image<T>::get(int i) const {
    return this->data[i];
}

template <class T>
void Volume<T>::set(int x, int y, int z, T value) {
    this->data[x+y*this->width+z*this->width*this->height] = value;
}

template <class T>
T Volume<T>::get(int x, int y, int z) const {
    return this->data[x+y*this->width+z*this->width*this->height];
}

template <class T>
T Volume<T>::get(int i) const {
    return this->data[i];
}



template <class T>
Window<T>::Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Image<T> * image) {
	this->gtkWindow = gtkWindow;
    this->gtkImage = gtkImage;
    this->image = image;
    this->isVolume = false;
    this->level = -1.0f;
}

template <class T>
Window<T>::Window(GtkWidget * gtkWindow, GtkWidget * gtkImage, Volume<T> * volume) {
	this->gtkWindow = gtkWindow;
    this->gtkImage = gtkImage;
    this->volume = volume;
    this->isVolume = true;
    this->level = -1.0f;
}

template <class T>
void Window<T>::update() {
    if(this->isVolume) {
        if(this->level == -1.0f) {
            volume->dataToPixbuf(this->gtkImage, this->currentSlice, this->currentDirection);
        } else {
            volume->dataToPixbuf(this->gtkImage,  this->currentSlice, this->currentDirection, this->level, this->window);
        }
    } else {
        if(this->level == -1.0f) {
            image->dataToPixbuf(this->gtkImage);
        } else {
            image->dataToPixbuf(this->gtkImage, this->level, this->window);
        }
    }
    gtk_widget_queue_draw(this->gtkImage);
}

template <class T>
void Window<T>::destroy() {
	gtk_widget_destroy(GTK_WIDGET(this->gtkWindow));
}

struct _saveData {
	GtkWidget * fs;
	GtkImage * image;
};

template <class T>
gboolean Image<T>::setupGUI(gpointer data) {
	int windowCount = increaseWindowCount();
	gdk_threads_enter();
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window),
			("Image #" + intToString(windowCount)).c_str()
	);
	Window<T> * win = (Window<T> *)data;
	GtkWidget * toolbar = gtk_toolbar_new();
	gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar), GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_append_item (
			 GTK_TOOLBAR (toolbar), /* our toolbar */
             "Save",               /* button label */
             "Save this image",     /* this button's tooltip */
             NULL,             /* tooltip private info */
             NULL,                 /* icon widget */
             GTK_SIGNAL_FUNC(saveDialog), /* a signal */
             win->gtkImage);
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
			win->image->getWidth(),
			win->image->getHeight() + 35
	);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(
			G_OBJECT(window),
			"destroy",
			G_CALLBACK(destroyWindow),
			NULL
	);

	

	GtkWidget * fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (window), fixed);
	gtk_fixed_put(GTK_FIXED(fixed), toolbar, 0,0);
	gtk_fixed_put(GTK_FIXED(fixed), win->gtkImage, 0, 35);
	gtk_widget_show_all(window);

	
	win->gtkWindow = window;

	gdk_threads_leave();
	return false;
}



template <class T>
void Window<T>::wrapper_key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data) {
    Window<T> * win = (Window<T> *)user_data;
    win->key_pressed(widget, event, user_data);
}


template <class T>
void Window<T>::key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer user_data) {
    switch(event->keyval) {
        case GDK_KEY_Up:
            this->currentSlice = validateSlice(this->currentSlice+1,this->currentDirection,this->volume->getSize());
            break;
        case GDK_KEY_Down:
            this->currentSlice = validateSlice(this->currentSlice-1,this->currentDirection,this->volume->getSize());
            break;
    }
    if(level == -1.0f) {
        this->volume->dataToPixbuf(this->gtkImage, this->currentSlice, this->currentDirection);
    } else {
        this->volume->dataToPixbuf(this->gtkImage, this->currentSlice, this->currentDirection, level, window);
    }
    gtk_widget_queue_draw(this->gtkImage);
}


template <class T>
gboolean Volume<T>::setupGUI(gpointer data) {

    Window<T> * winObj = (Window<T> *)data;
	gdk_threads_enter();
    int windowCount = increaseWindowCount();
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    winObj->gtkWindow = window;
	gtk_window_set_title(GTK_WINDOW(window),
			("Volume #" + intToString(windowCount)).c_str()
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
             winObj->gtkImage);
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
			winObj->volume->getWidth(),
			winObj->volume->getHeight() + 35
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
            G_CALLBACK(Window<T>::wrapper_key_pressed),
            winObj
    );

	GtkWidget * fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (window), fixed);
	gtk_fixed_put(GTK_FIXED(fixed), toolbar, 0,0);
	gtk_fixed_put(GTK_FIXED(fixed), winObj->gtkImage, 0, 35);
	gtk_widget_show_all(window);

	gdk_threads_leave();
    return false;

}


template <class T>
Window<T> * Image<T>::show() {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
    this->dataToPixbuf(image);
	Window<T> * winObj = new Window<T>(NULL,image,this);
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Image<T>::setupGUI, winObj, NULL);
    return winObj;
}

template <class T>
Window<T> * Image<T>::show(float level, float window) {
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, width, height));
    this->dataToPixbuf(image, level, window);
	Window<T> * winObj = new Window<T>(NULL,image,this);
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Image<T>::setupGUI, winObj, NULL);
    return winObj;
}

template <class T>
Window<T> * Volume<T>::show(){
    int slice = this->depth/2;
    slice_plane direction = Z;
    int displayWidth = this->width;
    int displayHeight = this->height;
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, displayWidth, displayHeight));
    this->dataToPixbuf(image, slice, direction);
	Window<T> * winObj = new Window<T>(NULL,image,this);
    winObj->currentSlice = slice;
    winObj->currentDirection = direction;
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Volume<T>::setupGUI, winObj, NULL);
    return winObj;
}

template <class T>
Window<T> * Volume<T>::show(float level, float window){
    int slice = this->depth/2;
    slice_plane direction = Z;
    int displayWidth = this->width;
    int displayHeight = this->height;
    GtkWidget * image = gtk_image_new_from_pixbuf(gdk_pixbuf_new(GDK_COLORSPACE_RGB, false,
			8, displayWidth, displayHeight));
    this->dataToPixbuf(image, slice, direction,level,window);
	Window<T> * winObj = new Window<T>(NULL,image,this);
    winObj->currentSlice = slice;
    winObj->currentDirection = direction;
    winObj->level = level;
    winObj->window = window;
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Volume<T>::setupGUI, winObj, NULL);
    return winObj;
}


template <class T>
Window<T> * Volume<T>::show(int slice, slice_plane direction) {
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
	Window<T> * winObj = new Window<T>(NULL,image,this);
    winObj->currentSlice = slice;
    winObj->currentDirection = direction;
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Volume<T>::setupGUI, winObj, NULL);
    return winObj;
}

template <class T>
Window<T> * Volume<T>::show(int slice, slice_plane direction,float level, float window) {
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
    this->dataToPixbuf(image, slice, direction,level,window);
	Window<T> * winObj = new Window<T>(NULL,image,this);
    winObj->currentSlice = slice;
    winObj->currentDirection = direction;
    winObj->level = level;
    winObj->window = window;
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, Volume<T>::setupGUI, winObj, NULL);
    return winObj;
}


template <class T>
int Image<T>::getWidth() const {
    return this->width;
}

template <class T>
int Image<T>::getHeight() const {
    return this->height;
}

template <class T>
int Volume<T>::getWidth() const {
    return this->width;
}

template <class T>
int Volume<T>::getHeight() const {
    return this->height;
}

template <class T>
int Volume<T>::getDepth() const {
    return this->depth;
}

template <class T>
int2 Image<T>::getSize() const {
    int2 size;
    size.x = width;
    size.y = height;
    return size;
}
template <class T>
int3 Volume<T>::getSize() const {
    int3 size;
    size.x = width;
    size.y = height;
    size.z = depth;
    return size;
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
