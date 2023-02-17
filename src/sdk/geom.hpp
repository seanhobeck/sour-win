/**
 * 
 *      @author Sean Hobeck
 *       @date 02-15-2023
 * 
 **/
#pragma once

/// @uses: std::min, std::max, std::clamp
#include <bits/stdc++.h>


/// @brief Basic structure for a vector3
struct vector_t
{
    /// xyz, rgb, v, etc...
    union { struct { float x, y, z; }; struct { float r, g, b; }; float v[3]; };

    /// Explicit Constructors

    explicit vector_t() : x(0.f), y(0.f), z(0.f) {};
    explicit vector_t(int v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(float v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(double v[3]) : x(v[0]), y(v[1]), z(v[2]) {};
    explicit vector_t(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
    explicit vector_t(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    explicit vector_t(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

    /// Operators & Member Functions

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
};

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