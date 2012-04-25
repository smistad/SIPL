#include "Core.hpp"

namespace SIPL {
bool init = false;
pthread_t gtkThread;
void * initGTK(void * t) {
	gdk_threads_init ();
	gdk_threads_enter ();
	gtk_init(0, (char ***) "");
	init = true;
	gtk_main();
    gdk_threads_leave();
    return 0;
}
bool endNotCalled = true;
void End() {
    endNotCalled = false;
	pthread_join(gtkThread, NULL);
}

void Quit() {
    gtk_main_quit();
    endNotCalled = false;
	pthread_join(gtkThread, NULL);
    exit(0);
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


void quit(void) {
    if(endNotCalled)
        std::cout << "You forgot to call SIPL::End() in your program!" << std::endl;
}

int windowCount;
void Init() {
    windowCount = 0;
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
int increaseWindowCount() {
    windowCount ++;
    return windowCount;
}
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
    pixel[0] = value.x*255;
    pixel[1] = value.y*255;
    pixel[2] = value.z*255;
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
void convertImageType(float2 * to, color_float from) {
    to->x = from.red;
    to->y = from.green;
}
void convertImageType(float3 * to, color_float from) {
    to->x = from.red;
    to->y = from.green;
    to->z = from.blue;
}
} // End namespace
