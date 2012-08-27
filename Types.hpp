/**
 * Simple Image Processing Library
 * Copyright Erik Smistad 2012
 * See LICENSE file for information on use 
 */

#ifndef SIPL_TYPES
#define SIPL_TYPES

#include <math.h>

namespace SIPL {
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint; 
typedef struct color_float { float red, blue, green;} color_float ; 
typedef struct color_uchar { unsigned char red, blue, green;} color_uchar ;
enum slice_plane {X,Y,Z};

// Vector classes
class float2 {
    public:
        float x,y;
        float2() {
            this->x = 0.0f;
            this->y = 0.0f;
        };
        float2(float x, float y) {
            this->x = x;
            this->y = y;
        };
        float length() const {
            return sqrt(x*x+y*y);
        };
        float2 normalize() const {
            float l = this->length();
            return float2(x/l,y/l);
        };
        float distance(float2 &other) const {
            return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
        };
        float dot(float2 &other) const {
            return x*other.x+y*other.y;
        };
};

class float3 {
    public:
        float x,y,z;
        float3() {
            this->x = 0.0f;
            this->y = 0.0f;
            this->z = 0.0f;
        };
        float3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
        float length() const {
            return sqrt(x*x+y*y+z*z);
        };
        float3 normalize() {
            float l = this->length();
            return float3(x / l, y / l, z / l);
        };
        float distance(float3 &other) const {
            return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
        };
        float dot(float3 &other) const {
            return x*other.x+y*other.y+z*other.z;
        };
};

// These are not for images/volumes
class int2 {
    public:
        int x,y;
        int2() {
            this->x = 0;
            this->y = 0;
        };
        int2(float x, float y) {
            this->x = x;
            this->y = y;
        };
        float length() const {
            return sqrt(x*x+y*y);
        };
        float2 normalize() const {
            float l = this->length();
            return float2(x / l, y / l);
        };
        float distance(int2 &other) const {
            return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
        };
        float dot(int2 &other) const {
            return x*other.x+y*other.y;
        };
};

class int3 {
    public:
        int x,y,z;
        int3() {
            this->x = 0;
            this->y = 0;
            this->z = 0;
        };
        int3(int x, int y, int z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
        float length() const {
            return sqrt(x*x+y*y+z*z);
        };
        float3 normalize() const {
            float l = this->length();
            return float3(x / l, y / l, z / l);
        };
        float distance(int3 &other) const {
            return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
        };
        float dot(int3 &other) const {
            return x*other.x+y*other.y+z*other.z;
        };
};

} // end SIPL namespace
#endif
