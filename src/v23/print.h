/**
 * 
 * 
 *      @author  Sean Hobeck
 *       @date 2023-02-14
 * 
 * 
 **/
#pragma once

/// @uses: fwprintf, fprintf, FILE (__IO_FILE), std::ostream, std::cout, std::wcout, std::string, std::wstring
#include <bits/stdc++.h>

/// @uses: std::vformat, std::wformat
#include "format.h"


/// @brief Namespace for the "gnu/linux c++23 standard".
namespace __gnu_cxx::v23 {
    /// @brief Prints out to stdout, with formatted args.
    /// @tparam ...pargs_t Packed args (same as virtual arguments).
    /// @param __format The string to be formatted to.
    /// @param __args Format parameters to format the string and print to stdout.
    template<typename ... pargs_t>
    static void 
    print(const std::string& __format, pargs_t... __args) noexcept 
    { 
        std::cout << __gnu_cxx::v23::vformat(__format, __args...); 
    };
    /// @brief Prints out to a file stream, with formatted args.
    template<typename ... pargs_t>
    static void 
    print(const std::ostream& __fs, const std::string& __format, pargs_t... __args) noexcept 
    { 
        __fs << __gnu_cxx::v23::vformat(__format, __args...); 
    };


    /// @brief Prints a line out to stdout, with formatted args.
    /// @tparam ...pargs_t Packed args (same as virtual arguments).
    /// @param __format The string to be formatted to.
    /// @param __args Format parameters to format the string and print a line to stdout.
    template<typename ... pargs_t>
    static void 
    println(const std::string& __format, pargs_t... __args) noexcept 
    { 
        std::cout << __gnu_cxx::v23::vformat(__format, __args...) << std::endl; 
    };
    /// @brief Prints a line out to a file stream, with formatted args.
    template<typename ... pargs_t>
    static void 
    println(const std::ostream& __fs, const std::string& __format, pargs_t... __args) noexcept 
    { 
        __fs << __gnu_cxx::v23::vformat(__format, __args...) << std::endl;
    };


    /// @brief Writes out to a file pointer, with formatted args (unicode).
    /// @tparam ...pargs_t Packed args (same as virtual arguments).
    /// @param __fp 
    /// @param __format The string to be formatted to.
    /// @param __args Format parameters to format the string and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprint(FILE* __fp, const std::string& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::vformat(__format, __args...); __f.length() > 0)
            fwrite(__f.c_str(), __f.length(), 1ul, __fp);
    };
    /// @brief Writes out to stdout with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprint(const std::string& __format, pargs_t... __args) noexcept 
    { 
        vprint(stdout, __format, __args...);
    };
    
    
    /// @brief Writes out to a file pointer, ending the line, with formatted args (unicode).
    /// @tparam ...pargs_t Packed args (same as virtual arguments).
    /// @param __fp The file pointer.
    /// @param __format The string to be formatted to.
    /// @param __args Format parameters to format the string and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(FILE* __fp, const std::string& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::vformat(__format, __args...) + '\n'; __f.length() > 0)
            fprintf(__fp, __f.c_str());
    };
    /// @brief Writes out to stdout with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(const std::string& __format, pargs_t... __args) noexcept 
    { 
        vprintln_unicode(stdout, __format, __args...);
    };
    /// @brief Writes out to std::wostream with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(std::ostream& __stream, std::wstring& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::wformat(__format, __args...) + std::endl; __f.length() > 0)
            __stream << __f;
    };
        
    
    /// @brief Writes out to a file pointer, ending the line, with formatted args (non-unicode).
    /// @tparam ...pargs_t Packed args (same as virtual arguments).
    /// @param __fp 
    /// @param __format The wide string to be formatted to.
    /// @param __args Format parameters to format the wstring and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(FILE* __fp, const std::wstring& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::wformat(__format, __args...) + L'\n'; __f.length() > 0)
            fwprintf(__fp, __f.c_str());
    };
    /// @brief Writes out to stdout with formatted args (non-unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(const std::wstring& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::wformat(__format, __args...) + L'\n'; __f.length() > 0)
            std::wcout << __f;
    };
    /// @brief Writes out to std::wostream with formatted args (non-unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(std::wostream& __stream, std::wstring& __format, pargs_t... __args) noexcept 
    { 
        if (auto __f = __gnu_cxx::v23::wformat(__format, __args...) + L'\n'; __f.length() > 0)
            __stream << __f;
    };
};