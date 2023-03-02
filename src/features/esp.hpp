/**
 *
 *      @author Sean Hobeck
 *       @date 02-25-2023
 *
 **/
#pragma once

/// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "../sdk/entity.hpp"
#include "../util/draw.hpp"
#include "../util/log.hpp"


/// @brief Namespace for the Extra-Sensory Perception.
namespace esp 
{
	/// @note: if we are in thirdperson.
	static bool thirdperson = false;

	/// @note: if this module is toggled.
	static bool enabled = false;


	/// @note: Toggling thirdperson.
	static void toggle_tp()
	{
		thirdperson = !thirdperson;

		if (thirdperson)
			l::log("thirdperson toggled");
		else
			l::log("thirdperson un-toggled");
	};

	/// @brief Toggling the module.
	static void toggle() 
	{ 
		enabled = !enabled;

		if (enabled)
			l::log("esp toggled");
		else
			l::log("esp un-toggled");
	};

	/// @brief Drawing a player
	static void draw_player(const sdk::player_t* p_player) 
	{
		/// Getting the height, width, head, and feet position of our indexed player
		vector_t h = p_player->m_new, n = p_player->m_new;
		n.z -= 12.5f;

		/// World-to-screen positions
		point_t _n, _h;

		/// If we can see this player.
		if (d::wts(n, _n) && d::wts(h, _h)) {
			const float sz = (_h.y - _n.y);
			d::rect(_n, sz / 5.f, sz, d::color_t(0, 0, 0, 255), 3.f);
			if (!strstr(p_player->m_sz_team, g::p_local->m_sz_team))
				d::rect(_n, sz / 5.f, sz, d::color_t(255, 105, 25, 255));
			else
				d::rect(_n, sz / 5.f, sz, d::color_t(25, 155, 255, 255));

			/// Creating a healthbar on the side.
			point_t g = _n;
			g.x -= sz / 3.5f;
			float t = g.y + sz * 1.25f;
			float hp = sz * ((float)p_player->m_ihealth / (float)p_player->m_imaxhealth);
			d::line(g, point_t(g.x, g.y + sz), d::color_t(0, 0, 0, 255), 3.f);
			d::line(g, point_t(g.x, g.y + sz), d::color_t(255, 15, 25, 255));
			d::line(g, point_t(g.x, g.y + hp), d::color_t(15, 255, 25, 255));
		};
	};

	/// @brief Rendering to the screen all of the players except for us.
	static void loop() 
	{
		/// Toggle checking
		if (!enabled)
			return;

		/// Looping through the entitylist.
		for (std::size_t i = 0; i < *g::p_playercount; i++)
		{
			auto p_player = g::p_list->get_entity(i);

			/// Sanity checking.
			if (!p_player->is_valid() || p_player == g::p_local)
				continue;

			/// Drawing the player.
			draw_player(p_player);
		};
	};
};