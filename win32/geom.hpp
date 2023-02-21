/**
 *
 *      @author Sean Hobeck
 *       @date 02-15-2023
 *
 **/
#pragma once

 /// @uses: std::min, std::max, std::clamp
#include <cmath>
#include <vector>
#include <algorithm>
#include <optional>


/// @brief Basic structure for a vector2 / point
struct point_t
{
    /// xy, rg, etc...
    union { struct { float x, y; }; };

    ///--------------- Explicit Constructors -------------------///

    explicit point_t() : x(0.f), y(0.f) {};
    explicit point_t(int v[2]) : x(v[0]), y(v[1]) {};
    explicit point_t(float v[2]) : x(v[0]), y(v[1]) {};
    explicit point_t(double v[2]) : x(v[0]), y(v[1]) {};
    explicit point_t(int _x, int _y) : x(_x), y(_y) {};
    explicit point_t(float _x, float _y) : x(_x), y(_y) {};
    explicit point_t(double _x, double _y) : x(_x), y(_y) {};
};

/// @brief Basic structure for a vector3
struct vector_t
{
    /// xyz, rgb, v, etc...
    union { struct { float x, y, z; }; struct { float r, g, b; }; float v[3]; };

    ///--------------- Explicit Constructors -------------------///

    explicit vector_t() : x(0.f), y(0.f), z(0.f) {};
    explicit vector_t(int v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(float v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(double v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
    explicit vector_t(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    explicit vector_t(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

    ///----------- Operators & Member Functions ----------------///

    bool zero()
    {
        return x == 0.f && y == 0.f && z == 0.f;
    };
    void add(const vector_t& _u)
    {
        this->x += _u.x;
        this->y += _u.y;
        this->z += _u.z;
    };
    void sub(const vector_t& _u)
    {
        this->x -= _u.x;
        this->y -= _u.y;
        this->z -= _u.z;
    };
    void mul(const vector_t& _u)
    {
        this->x *= _u.x;
        this->y *= _u.y;
        this->z *= _u.z;
    };
    void mul(const float _u)
    {
        this->x *= _u;
        this->y *= _u;
        this->z *= _u;
    };
    void div(const vector_t& _u)
    {
        this->x /= _u.x;
        this->y /= _u.y;
        this->z /= _u.z;
    };
    void div(const float _u)
    {
        this->x /= _u;
        this->y /= _u;
        this->z /= _u;
    };
    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    };
};

/// @brief Basic structure for a vector4 / quaternion
struct quatern_t
{
    /// xyzw, rgba, etc...
    union { struct { float x, y, z, w; }; };

    ///---------------- Explicit Constructors -----------------///

    explicit quatern_t() : x(0.f), y(0.f), z(0.f), w(0.f) {};
    explicit quatern_t(int v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {};
    explicit quatern_t(float v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {};
    explicit quatern_t(double v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {};
    explicit quatern_t(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {};
    explicit quatern_t(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
    explicit quatern_t(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {};
};


///---------------- @section: Inline Operators ------------------///


inline vector_t operator+(const vector_t& _u, const vector_t& _v)
{
    return vector_t(_u.x + _v.x, _u.y + _v.y, _u.z + _v.z);
};
inline vector_t operator-(const vector_t& _u, const vector_t& _v)
{
    return vector_t(_u.x - _v.x, _u.y - _v.y, _u.z - _v.z);
};
inline bool operator==(const vector_t& _u, const vector_t& _v)
{
    return _u.x == _v.x && _u.y == _v.y && _u.z == _v.z;
};
inline bool operator!=(const vector_t& _u, const vector_t& _v)
{
    return _u.x != _v.x || _u.y != _v.y || _u.z != _v.z;
};