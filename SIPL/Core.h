/**
 *
 * @author Erik Smistad <smistad@idi.ntnu.no>
 */

#ifndef IPLIB_H_
#define IPLIB_H_

#include <stdlib.h>
#include <cmath>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

namespace SIPL
{
	void quit(void);
    void Init();

	struct pixel {
		unsigned char blue,green,red,alpha;
	};


	class Window
	{
	public:
		Window(GtkWidget * gtkWindow);
		void update();
		void destroy();
	private:
		GtkWidget * gtkWindow;
	};

	class Pixel;
	class Mask;

	class Image
	{
	public:
		Image(const char * filepath);
		Image(unsigned int width, unsigned int height);
		~Image();
		Pixel getPixel(int x, int y);
		int getWidth();
		int getHeight();
		Window show();
		void crop(); 
		Image convolution(Mask * mask); 
		void save(const char * filepath, const char * imageType);
		friend class Pixel;
	private:
		GtkWidget * image;
	};

	class Pixel
	{
	public:
		Pixel(pixel p, Image *image, int x, int y);
		void set(unsigned char intensity);
		void set(unsigned char red, unsigned char green, unsigned char blue);
		void set(pixel pixel);
		void set(Pixel pixel);
		unsigned char * get();
	private:
		unsigned char blue,green,red,alpha;
		Image *image;
		int x, y;
	};

	class Mask
	{
	public:
		double ** get() {createMatrix(); return maskMatrix; };
		int getSize() { return this->size; };
	protected:
		double ** maskMatrix;
		int size;
		virtual void createMatrix() = 0;
	};

}
#endif /* IPLIB_H_ */
