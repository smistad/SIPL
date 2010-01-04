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
	void setupWindow(int width, int height);
	void quit(void);

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
		void update(int x, int y, int w, int h);
		Window * show();
		void crop(); // TODO:
		Image * convolution(Mask * mask); // TODO:
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

	class AverageMask : public Mask
	{
	public:
		AverageMask(int size) { this->size = size; }
		void createMatrix()
		{
			maskMatrix = (double **)malloc(sizeof(double)*size);
			for(int i = 0; i < size; i++) {
				maskMatrix[i] = (double *)malloc(sizeof(double)*size);
				for(int j = 0; j < size; j++) {
					maskMatrix[i][j] = 1.0/(double)(size*size);
				}
			}
		};
	};

	class LaplaceMask : public Mask
	{
	public:
		void createMatrix()
		{
			size = 3;
			maskMatrix = (double **)malloc(sizeof(double)*size);
			for(int i = 0; i < size; i++) {
				maskMatrix[i] = (double *)malloc(sizeof(double)*size);
			}
			maskMatrix[0][0] = -1.0;
			maskMatrix[0][1] = -1.0;
			maskMatrix[0][2] = -1.0;
			maskMatrix[2][0] = -1.0;
			maskMatrix[2][1] = -1.0;
			maskMatrix[2][2] = -1.0;
			maskMatrix[0][0] = -1.0;
			maskMatrix[1][0] = -1.0;
			maskMatrix[2][0] = -1.0;
			maskMatrix[0][2] = -1.0;
			maskMatrix[1][2] = -1.0;
			maskMatrix[2][2] = -1.0;
			maskMatrix[1][1] = 8.0;
		}
	};

}
#endif /* IPLIB_H_ */
