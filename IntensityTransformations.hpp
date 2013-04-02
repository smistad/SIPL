#ifndef INTENSITYTRANSFORMATIONS_HPP_
#define INTENSITYTRANSFORMATIONS_HPP_

#include "Types.hpp"
#include "Exceptions.hpp"
#include <typeinfo>
#include <iostream>
namespace SIPL {

enum TransformationType { NONE, HOUNSEFIELD, GRAYSCALE, NORMALIZED, CUSTOM };

class IntensityTransformation {
private:
	TransformationType type;
	void(*func)(void *, void *, unsigned int, unsigned int);
public:
	IntensityTransformation() { this->type = NONE; };
	IntensityTransformation(void(*func)(void *, void *, unsigned int, unsigned int)) {
		this->func = func;
		this->type = CUSTOM;
	};

	IntensityTransformation(TransformationType type) { this->type = type; };
	template <class S, class T>
	void transform(S * from, T * to, unsigned int length, unsigned int start = 0) {
		switch(type) {
		case HOUNSEFIELD:
			// Target must be short
			if(typeid(T) != typeid(short))
				throw SIPLException("Has to be short.");

			if(typeid(S) == typeid(short)) {
				// Do a simple copy
				this->copy(from, to, length, start);
			} else if(typeid(S) == typeid(ushort)) {
				for(int i = start; i < start+length; i++) {
					to[i] = from[i] - 1024;
				}
			} else {
				throw SIPLException("Source has to be ushort or short");
			}

			break;
		case GRAYSCALE:
			break;
		case CUSTOM:
			func(from,to,length,start);
			break;
		case NORMALIZED:
			if(typeid(T) != typeid(float)) {
				throw SIPLException("Target has to float");
			} else {

			// find min and max

			float min = from[start];
			float max = from[start];
			for(int i = start; i < start+length; i++) {
				if(from[i] < min)
					min = from[i];
				if(from[i] > max)
					max = from[i];
			}
			for(int i = start; i < start+length; i++) {
				to[i] = ((float)from[i]-min)/(max+min);
			}
			}
			break;
		case NONE:
			this->copy(from, to, length, start);
			break;
		default:
			throw SIPLException("Invalid transformation type");
			break;
		}
	}
	template <class S, class T>
	static void copy(S * from, T * to, unsigned int length, unsigned int start) {
		for(int i = start; i < length+start; i++) {
			to[i] = from[i];
		}
	}
};

}
#endif /* INTENSITYTRANSFORMATIONS_HPP_ */
