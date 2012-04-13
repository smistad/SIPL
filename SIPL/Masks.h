#include "Core.h"

namespace SIPL {

Image Image::convolution(Mask * mask) {
	double ** maskMatrix = mask->get();
	int width = this->getWidth();
	int height = this->getHeight();
	int size = mask->getSize();
	Image newImage = Image(width, height);
	double sum = 0.0;

	for (int y = size / 2; y < height - (size / 2); y++) {
		for (int x = size / 2; x < width - (size / 2); x++) {
			sum = 0.0;
			for (int m = 0; m < size; m++) {
				for (int n = 0; n < size; n++) {
					sum += maskMatrix[m][n] * this->getPixel(
							x + (size / 2) - m, y + (size / 2) - n).get()[0];
				}
			}

			newImage.getPixel(x, y).set((unsigned char) round(sum));
		}
	}

	return newImage;
}

class AverageMask: public Mask {
public:
	AverageMask(int size) {
		this->size = size;
	}
	void createMatrix() {
		maskMatrix = (double **) malloc(sizeof(double) * size);
		for (int i = 0; i < size; i++) {
			maskMatrix[i] = (double *) malloc(sizeof(double) * size);
			for (int j = 0; j < size; j++) {
				maskMatrix[i][j] = 1.0 / (double) (size * size);
			}
		}
	}
};

class LaplaceMask: public Mask {
public:
	void createMatrix() {
		size = 3;
		maskMatrix = (double **) malloc(sizeof(double) * size);
		for (int i = 0; i < size; i++) {
			maskMatrix[i] = (double *) malloc(sizeof(double) * size);
		}
		maskMatrix[0][0] = -1.0;
		maskMatrix[0][1] = -1.0;
		maskMatrix[0][2] = -1.0;
		maskMatrix[1][0] = -1.0;
		maskMatrix[1][1] = 8.0;
		maskMatrix[1][2] = -1.0;
		maskMatrix[2][0] = -1.0;
		maskMatrix[2][1] = -1.0;
		maskMatrix[2][2] = -1.0;
	}
};

class SobelXMask : public Mask {
public:
	void createMatrix()
	{
		size = 3;
		maskMatrix = (double **) malloc(sizeof(double) * size);
		for (int i = 0; i < size; i++) {
			maskMatrix[i] = (double *) malloc(sizeof(double) * size);
		}
		maskMatrix[0][0] = 1.0;
		maskMatrix[0][1] = 0.0;
		maskMatrix[0][2] = -1.0;
		maskMatrix[1][0] = 2.0;
		maskMatrix[1][1] = 0.0;
		maskMatrix[1][2] = -2.0;
		maskMatrix[2][0] = 1.0;
		maskMatrix[2][1] = 0.0;
		maskMatrix[2][2] = -1.0;
	}
};

class SobelYMask : public Mask {
public:
	void createMatrix()
	{
		size = 3;
		maskMatrix = (double **) malloc(sizeof(double) * size);
		for (int i = 0; i < size; i++) {
			maskMatrix[i] = (double *) malloc(sizeof(double) * size);
		}
		maskMatrix[0][0] = -1.0;
		maskMatrix[0][1] = -2.0;
		maskMatrix[0][2] = -1.0;
		maskMatrix[1][0] = 0.0;
		maskMatrix[1][1] = 0.0;
		maskMatrix[1][2] = 0.0;
		maskMatrix[2][0] = 1.0;
		maskMatrix[2][1] = 2.0;
		maskMatrix[2][2] = 1.0;
	}
};

}
