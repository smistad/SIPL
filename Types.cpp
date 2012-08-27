#include "Types.hpp"
namespace SIPL {

// float2
float float2::distance(float2 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
}
float float2::dot(float2 &other) const {
    return x*other.x+y*other.y;
}
float float2::distance(int2 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
}
float float2::dot(int2 &other) const {
    return x*other.x+y*other.y;
}

// float3
float float3::distance(float3 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
}
float float3::dot(float3 &other) const {
    return x*other.x+y*other.y+z*other.z;
}
float float3::distance(int3 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
}
float float3::dot(int3 &other) const {
    return x*other.x+y*other.y+z*other.z;
}

// int2
float int2::distance(float2 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
}
float int2::dot(float2 &other) const {
    return x*other.x+y*other.y;
}
float int2::distance(int2 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y));
}
float int2::dot(int2 &other) const {
    return x*other.x+y*other.y;
}

// int3
float int3::distance(float3 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
}
float int3::dot(float3 &other) const {
    return x*other.x+y*other.y+z*other.z;
}
float int3::distance(int3 &other) const {
    return sqrt((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z));
}
float int3::dot(int3 &other) const {
    return x*other.x+y*other.y+z*other.z;
}

// float2
template <>
float2 float2::operator+(float2 other) const {
    float2 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    return v;
}
template <>
float2 float2::operator+(int2 other) const {
    float2 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    return v;
}
template <>
float2 float2::operator-(float2 other) const {
    float2 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    return v;
}
template <>
float2 float2::operator-(int2 other) const {
    float2 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    return v;
}
template <>
float2 float2::operator*(float2 other) const {
    float2 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    return v;
}
template <>
float2 float2::operator*(int2 other) const {
    float2 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    return v;
}

template <class T>
float2 operator+(T scalar, float2 other) {
    return other+scalar;
}
template <class T>
float2 operator-(T scalar, float2 other) {
    return other-scalar;
}
template <class T>
float2 operator*(T scalar, float2 other) {
    return other*scalar;
}


// float3
template <>
float3 float3::operator+(float3 other) const {
    float3 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    v.z = this->z + other.z;
    return v;
}
template <>
float3 float3::operator+(int3 other) const {
    float3 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    v.z = this->z + other.z;
    return v;
}
template <>
float3 float3::operator-(float3 other) const {
    float3 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    v.z = this->z - other.z;
    return v;
}
template <>
float3 float3::operator-(int3 other) const {
    float3 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    v.z = this->z - other.z;
    return v;
}
template <>
float3 float3::operator*(float3 other) const {
    float3 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    v.z = this->z * other.z;
    return v;
}
template <>
float3 float3::operator*(int3 other) const {
    float3 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    v.z = this->z * other.z;
    return v;
}

template <class T>
float3 operator+(T scalar, float3 other) {
    return other+scalar;
}
template <class T>
float3 operator-(T scalar, float3 other) {
    return other-scalar;
}
template <class T>
float3 operator*(T scalar, float3 other) {
    return other*scalar;
}

// int2
template <>
int2 int2::operator+(float2 other) const {
    int2 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    return v;
}
template <>
int2 int2::operator+(int2 other) const {
    int2 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    return v;
}
template <>
int2 int2::operator-(float2 other) const {
    int2 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    return v;
}
template <>
int2 int2::operator-(int2 other) const {
    int2 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    return v;
}
template <>
int2 int2::operator*(float2 other) const {
    int2 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    return v;
}
template <>
int2 int2::operator*(int2 other) const {
    int2 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    return v;
}

template <class T>
int2 operator+(T scalar, int2 other) {
    return other+scalar;
}
template <class T>
int2 operator-(T scalar, int2 other) {
    return other-scalar;
}
template <class T>
int2 operator*(T scalar, int2 other) {
    return other*scalar;
}


// float3
template <>
int3 int3::operator+(float3 other) const {
    int3 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    v.z = this->z + other.z;
    return v;
}
template <>
int3 int3::operator+(int3 other) const {
    int3 v;
    v.x = this->x + other.x;
    v.y = this->y + other.y;
    v.z = this->z + other.z;
    return v;
}
template <>
int3 int3::operator-(float3 other) const {
    int3 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    v.z = this->z - other.z;
    return v;
}
template <>
int3 int3::operator-(int3 other) const {
    int3 v;
    v.x = this->x - other.x;
    v.y = this->y - other.y;
    v.z = this->z - other.z;
    return v;
}
template <>
int3 int3::operator*(float3 other) const {
    int3 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    v.z = this->z * other.z;
    return v;
}
template <>
int3 int3::operator*(int3 other) const {
    int3 v;
    v.x = this->x * other.x;
    v.y = this->y * other.y;
    v.z = this->z * other.z;
    return v;
}

template <class T>
int3 operator+(T scalar, int3 other) {
    return other+scalar;
}
template <class T>
int3 operator-(T scalar, int3 other) {
    return other-scalar;
}
template <class T>
int3 operator*(T scalar, int3 other) {
    return other*scalar;
}
};
