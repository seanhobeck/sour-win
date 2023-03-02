/**
 *
 *      @author Sean Hobeck
 *       @date 02-25-2023
 *
 **/
#pragma once

 /// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "../sdk/entity.hpp"
#include "../util/config.hpp"

/// @uses: std::unordered_map<_Kty, _Ty>, std::vector<_Ty>
#include <unordered_map>
#include <vector>



/// @brief Namespace for Legit Aimbot.
namespace aim
{
	/// @note: if this module is toggled.
	static bool enabled = false;

	/// @brief Nearest player to our FOV.
	static sdk::player_t* target = nullptr;


	/// @brief Toggling the module.
	static void 
	toggle()
	{
		enabled = !enabled;

		if (enabled)
			l::log("legit toggled");
		else
			l::log("legit un-toggled");
	};

	/// @brief Drawing FOV.
	static void
	draw_fov() 
	{
		/// Toggle checking
		if (!enabled)
			return;

		auto rad = config::config.legit_radius;
		auto col = config::config.legit_color;
		d::circle(point_t(g::p_width / 2, g::p_height / 2), rad, 0.75f, col);
	};

	/// @brief Distance Finding
	static void
	distance_cycle()
	{
		/// All Possible Points.
		std::unordered_map<sdk::player_t*, point_t> map{};

		for (std::size_t i = 0; i < *g::p_playercount; i++)
		{
			auto p_plr = g::p_list->get_entity(i);

			/// Sanity, Valid, and Team checking.
			if (!p_plr->is_valid() || p_plr == g::p_local || p_plr->m_sz_team == g::p_local->m_sz_team)
				continue;

			/// New position and vector2 version.
			vector_t p = p_plr->m_new;
			point_t v;

			/// If it is visible on screen then add it to the map.
			if (d::wts(p, v))
				map.try_emplace(p_plr, v);
		}

		/// Center of the screen.
		point_t center = point_t(g::p_width / 2, g::p_height / 2);
		float best = 1000.f;

		/// Looping through our possible vector
		for (auto [player, point] : map)
		{
			/// Delta point & distance.
			point_t delta = point_t(center.x - point.x, center.y - point.y);
			float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

			/// Is within the legit radius.
			if (dist > config::config.legit_radius)
				continue;

			/// Finding the best.
			if (dist < best)
			{
				best = dist;
				target = player;
			};
		};
	};

	/// @brief Finding the nearest player to our FOV circle.
	static void
	find()
	{
		/// Nearest is always nullptr.
		target = nullptr;

		distance_cycle();
	};
};