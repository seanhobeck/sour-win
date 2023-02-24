/**
 *
 *      @author Sean Hobeck
 *       @date 02-24-2023
 *
 **/
#pragma once

/// @uses: point_t, vector_t, quatern_t, etc..
#include "geom.hpp"

/// @uses: opengl32, SDL2, tlhelp32
#pragma comment(lib, "OpenGL32.lib")
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <intrin.h>


/// @brief Global namespace.
namespace g 
{
	/// @note: Pointer to the view matrix.
	static float* p_matrix;

	/// @note: Width & Height of the viewport.
	static std::int32_t p_width, p_height;
};

/// @brief Namespace for all things drawing.
namespace d
{
	/// @brief Structure for rgba color.
	struct color_t 
	{
		/// r, g, b, a
		std::int32_t r = 0, g = 0, b = 0, a = 0;

		///-------------- Constructor ----------------///

		color_t(std::int32_t _r, std::int32_t _g, std::int32_t _b, std::int32_t _a) : r(_r), g(_g), b(_b), a(_a) { };
	};

	/// @brief World to screen function.
	/// @returns If the world to screen was possible.
	static bool
	wts(vector_t o, point_t& _ref) 
	{
		quatern_t _mbp;
		_mbp.x = o.x * g::p_matrix[0] + o.y * g::p_matrix[4] + o.z * g::p_matrix[8] + g::p_matrix[12];
		_mbp.y = o.x * g::p_matrix[1] + o.y * g::p_matrix[5] + o.z * g::p_matrix[9] + g::p_matrix[13];
		_mbp.z = o.x * g::p_matrix[2] + o.y * g::p_matrix[6] + o.z * g::p_matrix[10] + g::p_matrix[14];
		_mbp.w = o.x * g::p_matrix[3] + o.y * g::p_matrix[7] + o.z * g::p_matrix[11] + g::p_matrix[15];

		if (_mbp.w < 0.1f)
			return false;

		point_t _t(0.f, 0.f);
		_t.x = _mbp.x / _mbp.w;
		_t.y = _mbp.y / _mbp.w;

		_ref.x = (g::p_width / 2 * _t.x) + (_t.x + g::p_width / 2);
		_ref.y = -(g::p_height / 2 * _t.y) + (_t.y + g::p_height / 2);
		return true;
	};
	
	static void 
	rect(const point_t& _orig, const float w, const float h, const color_t color, const float width = 1.5f)
	{
		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		glColor4ub(color.r, color.g, color.b, color.a);
		glVertex2f(_orig.x + w, _orig.y);
		glVertex2f(_orig.x + w, _orig.y + h);
		glVertex2f(_orig.x - w, _orig.y + h);
		glVertex2f(_orig.x - w, _orig.y);
		glVertex2f(_orig.x + w, _orig.y);
		glEnd();
	};

	static void
	line(const point_t& _to, const point_t& _from, const color_t color, const float width = 1.5f)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glColor4ub(color.r, color.g, color.b, color.a);
		glVertex2f(_from.x, _from.y);
		glVertex2f(_to.x, _to.y);
		glEnd();
	};
};