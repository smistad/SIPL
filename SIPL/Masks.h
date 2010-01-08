#include "Core.h"
using namespace SIPL;
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
