/**
 * Simple Image Processing Library
 * Copyright Erik Smistad 2012
 * See LICENSE file for information on use 
 */
#include "Core.hpp"

namespace SIPL {
static bool init = false;
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
		g_thread_init(NULL);
		gtkThread = g_thread_create(initGTK, NULL, true, NULL);
	}
	while(!init) // wait for the thread to be created
	{
		//needed to prevent never ending while
		usleep(0);
	}
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

void toGuchar(bool value, guchar * pixel) {
    uchar intensity = value ? 255 : 0;
    pixel[0] = intensity;
    pixel[1] = intensity;
    pixel[2] = intensity;
}
void toGuchar(uchar value, guchar * pixel) {
    pixel[0] = value;
    pixel[1] = value;
    pixel[2] = value;
}
void toGuchar(char value, guchar * pixel) {
    pixel[0] = value+128;
    pixel[1] = value+128;
    pixel[2] = value+128;
}
void toGuchar(ushort value, guchar * pixel) {
    pixel[0] = ((float)value/65535)*255;
    pixel[1] = ((float)value/65535)*255;
    pixel[2] = ((float)value/65535)*255;
}
void toGuchar(short value, guchar * pixel) {
    pixel[0] = ((float)(value+32768)/65535)*255;
    pixel[1] = ((float)(value+32768)/65535)*255;
    pixel[2] = ((float)(value+32768)/65535)*255;
}
void toGuchar(uint value, guchar * pixel) {
    pixel[0] = ((float)value/4294967295)*255;
    pixel[1] = ((float)value/4294967295)*255;
    pixel[2] = ((float)value/4294967295)*255;
}
void toGuchar(int value, guchar * pixel) {
    pixel[0] = ((float)(value+2147483648)/4294967295)*255;
    pixel[1] = ((float)(value+2147483648)/4294967295)*255;
    pixel[2] = ((float)(value+2147483648)/4294967295)*255;
}
void toGuchar(float value, guchar * pixel) {
    pixel[0] = value*255;
    pixel[1] = value*255;
    pixel[2] = value*255;
}
void toGuchar(color_uchar value, guchar * pixel) {
    pixel[0] = value.red;
    pixel[1] = value.green;
    pixel[2] = value.blue;
}
void toGuchar(color_float value, guchar * pixel) {
    pixel[0] = value.red*255;
    pixel[1] = value.green*255;
    pixel[2] = value.blue*255;
}
void toGuchar(float2 value, guchar * pixel) {
    pixel[0] = fabs(value.x)*255;
    pixel[1] = fabs(value.y)*255;
    pixel[2] = 0;
}
void toGuchar(float3 value, guchar * pixel) {
    pixel[0] = fabs(value.x)*255;
    pixel[1] = fabs(value.y)*255;
    pixel[2] = fabs(value.z)*255;
}


void toGuchar(uchar value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(char value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(ushort value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(short value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(uint value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(int value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(float value, guchar * pixel, float level, float window) {
    uchar n = levelWindow(value, level, window);
    pixel[0] = n;
    pixel[1] = n;
    pixel[2] = n;
}
void toGuchar(color_float value, guchar * pixel, float level, float window) {
    pixel[0] = levelWindow(value.red, level, window);
    pixel[1] = levelWindow(value.green, level, window);
    pixel[2] = levelWindow(value.blue, level, window);
}
void toGuchar(color_uchar value, guchar * pixel, float level, float window) {
    pixel[0] = levelWindow(value.red, level, window);
    pixel[1] = levelWindow(value.green, level, window);
    pixel[2] = levelWindow(value.blue, level, window);
}
void toGuchar(float2 value, guchar * pixel, float level, float window) {
    pixel[0] = levelWindow(fabs(value.x), level, window);
    pixel[1] = levelWindow(fabs(value.y), level, window);
    pixel[2] = 0;
}
void toGuchar(float3 value, guchar * pixel, float level, float window) {
    pixel[0] = levelWindow(value.x, level, window);
    pixel[1] = levelWindow(value.y, level, window);
    pixel[2] = levelWindow(value.z, level, window);
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

// Conversion from bool
void convertImageType(bool * to, bool from) {
    *to = from;
}
void convertImageType(uchar * to, bool from) {
    *to = from ? 255 : 0;
}
void convertImageType(char * to, bool from) {
    *to = from ? 127 : -128;
}
void convertImageType(ushort * to, bool from) {
    *to = from ? 1 : 0;
}
void convertImageType(short * to, bool from) {
    *to = from ? 1 : 0;
}
void convertImageType(uint * to, bool from) {
    *to = from ? 1 : 0;
}
void convertImageType(int * to, bool from) {
    *to = from ? 1 : 0;
}
void convertImageType(float * to, bool from) {
    *to = from ? 1 : 0;
}
void convertImageType(color_float * to, bool from) {
    to->red = from ? 1 : 0;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, bool from) {
    to->red = from ? 255 : 0;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, bool from) {
    to->x = from ? 1 : 0;
    to->y = to->x;
}
void convertImageType(float3 * to, bool from) {
    to->x = from ? 1 : 0;
    to->y = to->x;
    to->z = to->x;
}

// Conversion from uchar
void convertImageType(bool * to, uchar from) {
    *to = from > 127;
}
void convertImageType(uchar * to, uchar from) {
    *to = from;
}
void convertImageType(char * to, uchar from) {
    *to = from-128;
}
void convertImageType(ushort * to, uchar from) {
    *to = from;
}
void convertImageType(short * to, uchar from) {
    *to = from;
}
void convertImageType(uint * to, uchar from) {
    *to = from;
}
void convertImageType(int * to, uchar from) {
    *to = from;
}
void convertImageType(float * to, uchar from) {
    *to = (float)from/255.0f;
}
void convertImageType(color_float * to, uchar from) {
    to->red = (float)from/255.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, uchar from) {
    to->red = from;
    to->blue = from;
    to->green = from;
}
void convertImageType(float2 * to, uchar from) {
    to->x = (float)from/255.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, uchar from) {
    to->x = (float)from/255.0f;
    to->y = to->x;
    to->z = to->x;
}


// Conversion from char
void convertImageType(bool * to, char from) {
    *to = from > 0;
}
void convertImageType(uchar * to, char from) {
    *to = from+128;
}
void convertImageType(char * to, char from) {
    *to = from;
}
void convertImageType(ushort * to, char from) {
    *to = from;
}
void convertImageType(short * to, char from) {
    *to = from;
}
void convertImageType(uint * to, char from) {
    *to = from;
}
void convertImageType(int * to, char from) {
    *to = from;
}
void convertImageType(float * to, char from) {
    *to = (float)from/255.0f+0.5f;
}
void convertImageType(color_float * to, char from) {
    to->red = (float)from/255.0f+0.5f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, char from) {
    to->red = from+128;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, char from) {
    to->x = (float)from/255.0f+0.5f;
    to->y = to->x;
}
void convertImageType(float3 * to, char from) {
    to->x = (float)from/255.0f+0.5;
    to->y = to->x;
    to->z = to->x;
}

// Conversion from ushort
void convertImageType(bool * to, ushort from) {
    *to = from > 0;
}
void convertImageType(uchar * to, ushort from) {
    *to = from/255;
}
void convertImageType(char * to, ushort from) {
    *to = from/255-128;
}
void convertImageType(ushort * to, ushort from) {
    *to = from;
}
void convertImageType(short * to, ushort from) {
    *to = from-32768;
}
void convertImageType(uint * to, ushort from) {
    *to = from;
}
void convertImageType(int * to, ushort from) {
    *to = from;
}
void convertImageType(float * to, ushort from) {
    *to = (float)from/65535.0f;
}
void convertImageType(color_float * to, ushort from) {
    to->red = (float)from/65535.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, ushort from) {
    to->red = ((float)from/65535)*255;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, ushort from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, ushort from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
    to->z = to->x;
}


// Conversion from short
void convertImageType(bool * to, short from) {
    *to = from > 0;
}
void convertImageType(uchar * to, short from) {
    *to = from/255;
}
void convertImageType(char * to, short from) {
    *to = from/255-128;
}
void convertImageType(ushort * to, short from) {
    *to = from+32768;
}
void convertImageType(short * to, short from) {
    *to = from;
}
void convertImageType(uint * to, short from) {
    *to = from;
}
void convertImageType(int * to, short from) {
    *to = from;
}
void convertImageType(float * to, short from) {
    *to = ((float)from/65535.0f)+0.5f;
}
void convertImageType(color_float * to, short from) {
    to->red = (float)from/65535.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, short from) {
    to->red = ((float)from/65535)*255;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, short from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, short from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
    to->z = to->x;
}

// Conversion from int
void convertImageType(bool * to, int from) {
    *to = from > 0;
}
void convertImageType(uchar * to, int from) {
    *to = from/255;
}
void convertImageType(char * to, int from) {
    *to = from/255-128;
}
void convertImageType(ushort * to, int from) {
    *to = from+32768;
}
void convertImageType(short * to, int from) {
    *to = from;
}
void convertImageType(uint * to, int from) {
    *to = from;
}
void convertImageType(int * to, int from) {
    *to = from;
}
void convertImageType(float * to, int from) {
    *to = ((float)from/4294967295.0f)+0.5f;
}
void convertImageType(color_float * to, int from) {
    to->red = (float)from/65535.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, int from) {
    to->red = ((float)from/65535)*255;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, int from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, int from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
    to->z = to->x;
}


// Conversion from uint
void convertImageType(bool * to, uint from) {
    *to = from > 0;
}
void convertImageType(uchar * to, uint from) {
    *to = from/255;
}
void convertImageType(char * to, uint from) {
    *to = from/255-128;
}
void convertImageType(ushort * to, uint from) {
    *to = from+32768;
}
void convertImageType(short * to, uint from) {
    *to = from;
}
void convertImageType(uint * to, uint from) {
    *to = from;
}
void convertImageType(int * to, uint from) {
    *to = from;
}
void convertImageType(float * to, uint from) {
    *to = ((float)from/4294967295.0f);
}
void convertImageType(color_float * to, uint from) {
    to->red = (float)from/65535.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, uint from) {
    to->red = ((float)from/65535)*255;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, uint from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, uint from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
    to->z = to->x;
}

// Conversion from color_uchar
void convertImageType(bool * to, color_uchar from) {
    *to = 0.33f*(from.red+from.blue+from.green) > 128 ;
}
void convertImageType(uchar * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green));
}
void convertImageType(char * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green)-128);
}
void convertImageType(ushort * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green)/255.0f)*65535;
}
void convertImageType(short * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green)/255.0f - 0.5f)*65535;
}
void convertImageType(uint * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green)/255.0f)*4294967295;
}
void convertImageType(int * to, color_uchar from) {
    *to = (0.33f*(from.red+from.blue+from.green)/255.0f-0.5f)*4294967295;
}
void convertImageType(float * to, color_uchar from) {
    *to = 0.33f*(from.red+from.blue+from.green)/255.0f;
}
void convertImageType(color_float * to, color_uchar from) {
    to->red = (float)from.red/255.0f;
    to->blue = (float)from.blue/255.0f;
    to->green =(float)from.green/255.0f;
}
void convertImageType(color_uchar * to, color_uchar from) {
    *to = from;
}
void convertImageType(float2 * to, color_uchar from) {
    to->x = (float)from.red/255.0f;
    to->y = (float)from.green/255.0f;
}
void convertImageType(float3 * to, color_uchar from) {
    to->x = (float)from.red/255.0f;
    to->y = (float)from.green/255.0f;
    to->z = (float)from.blue/255.0f;
}
// Conversion from color_float
void convertImageType(bool * to, color_float from) {
    *to = 0.33f*(from.red+from.blue+from.green) > 0.5f;
}
void convertImageType(uchar * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green))*255;
}
void convertImageType(char * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green)-0.5f)*255;
}
void convertImageType(ushort * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green))*65535;
}
void convertImageType(short * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green)-0.5f)*65535;
}
void convertImageType(uint * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green))*4294967295;
}
void convertImageType(int * to, color_float from) {
    *to = (0.33f*(from.red+from.blue+from.green)-0.5f)*4294967295;
}
void convertImageType(float * to, color_float from) {
    *to = 0.33f*(from.red+from.blue+from.green);
}
void convertImageType(color_float * to, color_float from) {
    *to = from;
}
void convertImageType(color_uchar * to, color_float from) {
    to->red = (float)from.red*255.0f;
    to->blue = (float)from.blue*255.0f;
    to->green = (float)from.green*255.0f;
}
void convertImageType(float2 * to, color_float from) {
    to->x = from.red;
    to->y = from.green;
}
void convertImageType(float3 * to, color_float from) {
    to->x = from.red;
    to->y = from.green;
    to->z = from.blue;
}

// Conversion from float
void convertImageType(bool * to, float from) {
    *to = from > 0;
}
void convertImageType(uchar * to, float from) {
    *to = from/255;
}
void convertImageType(char * to, float from) {
    *to = from/255-128;
}
void convertImageType(ushort * to, float from) {
    *to = from+32768;
}
void convertImageType(short * to, float from) {
    *to = from;
}
void convertImageType(uint * to, float from) {
    *to = from;
}
void convertImageType(int * to, float from) {
    *to = from;
}
void convertImageType(float * to, float from) {
    *to = from;
}
void convertImageType(color_float * to, float from) {
    to->red = (float)from/65535.0f;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(color_uchar * to, float from) {
    to->red = ((float)from/65535)*255;
    to->blue = to->red;
    to->green = to->red;
}
void convertImageType(float2 * to, float from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
}
void convertImageType(float3 * to, float from) {
    to->x = (float)from/65535.0f;
    to->y = to->x;
    to->z = to->x;
}



// Conversion from float2
void convertImageType(float * to, float2 from) {
    *to = sqrt(from.x*from.x+from.y*from.y);
}
void convertImageType(color_float * to, float2 from) {
    to->red = from.x;
    to->blue = from.y;
    to->green = 0;
}
void convertImageType(color_uchar * to, float2 from) {
    to->red = (float)from.x*255.0f;
    to->blue = (float)from.y*255.0f;
    to->green = 0;
}
void convertImageType(float2 * to, float2 from) {
    *to = from;
}
void convertImageType(float3 * to, float2 from) {
    to->x = from.x;
    to->y = from.y;
    to->z = 0;
}

// Conversion from float3
void convertImageType(float * to, float3 from) {
    *to = sqrt(from.x*from.x+from.y*from.y+from.z*from.z);
}
void convertImageType(color_float * to, float3 from) {
    to->red = from.x;
    to->blue = from.y;
    to->green = from.z;
}
void convertImageType(color_uchar * to, float3 from) {
    to->red = (float)from.x*255.0f;
    to->blue = (float)from.y*255.0f;
    to->green = (float)from.z*255.0f;
}
void convertImageType(float2 * to, float3 from) {
    to->x = from.x;
    to->y = from.y;
}
void convertImageType(float3 * to, float3 from) {
    *to = from;
}
} // End namespace
