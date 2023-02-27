/**
 *
 *      @author Sean Hobeck
 *       @date 02-21-2023
 *
 **/
#pragma once

/// @uses: std::cout, std::ostream, fprintf, etc..
#include <string>
#include <filesystem>

/// @uses: _getcwd
#include <direct.h>

/// @uses: log:: [namespace], d:: [namespace]
#include "log.hpp"
#include "draw.hpp"

/// @brief Namespace for all things considered configuration wise.
namespace config 
{
	/// @brief Structure for the configuration of sour.
	struct config_t 
	{
		/// Legit FOV Radius
		float legit_radius = 225;
		/// Legit FOV Color
		d::color_t legit_color = d::color_t(15, 255, 85, 255);

		/// @note: Configuration default.
		config_t() = default;
	};

	/// @brief Global Configuration.
	static config_t config = config_t();


	///----------- @section Config Functions --------------///


	/// @returns The current working directory.
	static std::string 
	cwd() 
	{
		char _buf[260ul];
		_getcwd(_buf, 260ul);
		return std::string(_buf);
	};

	/// @brief If a configuration already exists.
	static bool
	exists() 
	{
		/// Getting the current working directory & the config directory.
		auto _cwd = cwd();
		
		if (std::filesystem::exists(_cwd + "\\sour")) {
			l::log("config found");
			return true;
		}

		l::log("config not found");
		return false;
	};

	/// @brief Writing a configuration out to the filestream.
	static bool 
	write(std::ofstream& out, const config_t& _conf) 
	{
	};


	/// @brief Initializing our config.
	static void
	init()
	{
		/// Sour hack directory.
		auto _cwd = cwd();
		auto _dir = _cwd + "\\sour";

		/// Creating config directory.
		if (!exists()) {
			l::log("created config");
			CreateDirectoryA(_dir.c_str(), 0);
		}
	};
};