/**
 *
 *      @author Sean Hobeck
 *       @date 02-21-2023
 *
 **/
#pragma once

/// @uses: std::cout, std::ostream, fprintf, etc..
#include <iostream>
#include <fstream>
#include <cstdio>

/// @brief Namespace for all things considered configuration wise.
namespace config 
{
	/// @brief Structure for the configuration of sour.
	struct config_t 
	{
		
	};

	/// @brief If a configuration already exists.
	static bool exists(config_t& _conf) 
	{
	};

	/// @brief Writing a configuration out to the filestream.
	static bool write(std::ofstream& out, const config_t& _conf) 
	{
	};
};