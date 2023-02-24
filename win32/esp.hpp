/**
 *
 *      @author Sean Hobeck
 *       @date 02-24-2023
 *
 **/
#pragma once

 /// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "entity.hpp"
#include "draw.hpp"
#include "log.hpp"


/// @brief Namespace for the Extra-Sensory Perception.
namespace esp 
{
	/// @note: if this module is toggled.
	static bool gb_t = false;

	/// @brief Toggling the module.
	static void toggle() 
	{ 
		gb_t = !gb_t;
	};


	/// @brief Rendering to the screen all of the players except for us.
	static void render() 
	{
		/// Toggle checking
		if (!gb_t)
			return;

		/// Looping through the entitylist.
		for (std::size_t i = 0; i < *g::p_playercount; i++)
		{
			auto p_plr = g::p_list->get_entity(i);

			/// Sanity checking
			if (!p_plr->is_valid() || p_plr == g::p_local)
				continue;

			/// Getting the height, width, head, and feet position of our indexed player
			vector_t h = p_plr->m_new, n = p_plr->m_new;
			n.z -= 12.5f;

			/// World-to-screen positions
			point_t _n, _h;

			/// If we can see this player.
			if (d::wts(n, _n) && d::wts(h, _h)) {
				const float sz = (_h.y - _n.y);
				d::rect(_n, sz / 5.f, sz, d::color_t(0, 0, 0, 255), 3.f);
				d::rect(_n, sz / 5.f, sz, d::color_t(245, 125, 25, 255));

				/// Creating a healthbar on the side.
				point_t g = _n;
				g.x -= sz / 3.5f;
				float hp = sz * (p_plr->m_ihealth / p_plr->m_imaxhealth);
				d::line(g, point_t(g.x, g.y + sz), d::color_t(0, 0, 0, 255), 3.f);
				d::line(g, point_t(g.x, g.y + sz), d::color_t(255, 15, 25, 255));
				d::line(g, point_t(g.x, g.y + hp), d::color_t(15, 255, 25, 255));
			};
		};
	};
};