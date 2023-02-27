/**
 *
 *      @author Sean Hobeck
 *       @date 02-20-2023
 *
 **/
#pragma once

/// @uses: FILE*, freopen_s, std::string, std::string_view
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <string_view>

/// Defining your sour version.
#define SOUR_VERSION "v2.3.5"

/// @brief: Namespace for all things logging.
namespace l
{
	/// Pointer to the file
	static FILE* fptr;

	/// @brief Creating a console.
	static void
	create(const std::string_view sz_console_title) 
	{
		/// Allocating the console and setting the title.
		AllocConsole();
		SetConsoleTitleA(sz_console_title.data());

		/// stdout, stderr, stdlog, etc..
		freopen_s(&l::fptr, "CONOUT$", "w", stdout);
	};

	/// @brief Logging out to the console.
	template<typename T = const std::string>
	static void log(T _msg) 
	{
		std::cout << "[";  
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED | FOREGROUND_INTENSITY));
		std::cout << "sour-" << SOUR_VERSION;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
		std::cout << "]: ";
		std::cout << _msg << std::endl;
	};
};