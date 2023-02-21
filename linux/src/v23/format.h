/**
 * 
 * 
 *      @author  Sean Hobeck
 *       @date 2023-02-09
 * 
 * 
 **/
#pragma once

/// @uses: std::snprintf(), std::string
#include <string>

/// @uses: std::shared_ptr, std::unique_ptr
#include <memory>

/// @uses: std::optional, std::nullopt
#include <optional>

/// @uses: std::runtime_error
#include <stdexcept>


/// @brief Namespace for the "gnu/linux c++23 standard".
namespace __gnu_cxx::v23 {
    /// @brief A format error.
    class format_error : public std::runtime_error 
    {
    public:

        /// @brief Explicit Constructor
        /// @param what What the error is.
        explicit format_error(const std::string& what) : std::runtime_error(what) { };

        /// @brief Explicit Constructor
        /// @param what What the error is.
        explicit format_error(const char* what) : std::runtime_error(what) { };
    };



    ///--------------------------------------------- @section Unicode string formatting -------------------------------------------------------///



    /// @brief Formats a string (with a specified length).
    /// @tparam pargs_t A template for packed arguments (no va_args).
    /// @param __format The string that is going to be formatted.
    /// @param __n The length of characters you want to format.
    /// @param ...__args Virtual packed arguments to format.
    /// @return Returns a formatted string if the length isn't 0, otherwise it returns a empty string.
    template<typename ... pargs_t>
    static std::string
    _GLIBCXX_NODISCARD
    vnformat(const std::string& __format, std::size_t __n, pargs_t... __args) 
    {
        /// Checking the length of the string.
        if(auto sz = (std::size_t) std::snprintf( nullptr, 0, __format.c_str(), __args...) + 1; sz > 0 && __n != 0)
        {
            /// Initialize a temporary buffer.
            std::unique_ptr<char[]> buf(new char[__n]);

            /// Use std::snprintf() and return the string.
            std::snprintf(buf.get(), __n, __format.c_str(), __args...);
            return std::string(buf.get(), buf.get() + __n - 1);
        }

        ///  Return a empty string.
        return std::string();
    };
    /// @brief Formats a string.
    /// @tparam pargs_t A template for packed arguments (no va_args).
    /// @param __format The string that is going to be formatted.
    /// @param ...__args Virtual packed arguments to format.
    /// @return Returns a formatted string if the length isn't 0, otherwise it returns a empty string.
    template<typename ... pargs_t>
    static std::string
    _GLIBCXX_NODISCARD
    vformat(const std::string __format, pargs_t... __args) 
    {
        /// Checking the length of the string.
        if(auto sz = (std::size_t) std::snprintf( nullptr, 0, __format.c_str(), __args...) + 1; sz > 0)
        {
            /// Initialize a temporary buffer.
            std::unique_ptr<char[]> buf(new char[sz]);

            /// Use std::snprintf() and return the string.
            std::snprintf(buf.get(), sz, __format.c_str(), __args...);
            return std::string(buf.get(), buf.get() + sz - 1);
        }

        ///  Return a empty string.
        return std::string();
    };
    /// @brief Formats a string (with std::optional).
    /// @return Returns a std::optional of the formatted string.
    template<typename ... pargs_t>
    static const std::optional<std::string>
    _GLIBCXX_NODISCARD 
    voformat(const std::string& __format, pargs_t... __args) 
    {
        /// Call vformat() and check if its 0.
        auto s = vformat(__format, __args...);

        /// If it is not 0 then we make a optional out of it and return it, otherwise we return std::nullopt.
        return s.length() == 0 ? std::make_optional(s) : std::nullopt;
    };
    /// @brief Formats a string to a buffer.
    /// @param __buf The buffer to be formatted to.
    template<typename ... pargs_t>
    static void 
    vsformat(std::shared_ptr<std::string> __buf, const std::string& __format, pargs_t... __args) 
    {
        /// Wrapping the function, no need to overcomplicate things.
        __buf = vformat(__format, __args...);
    };
    /// @brief Formats a string to a buffer.
    /// @param __n
    /// @param __buf The buffer to be formatted to.
    template<typename ... pargs_t>
    static void 
    vsnformat(std::shared_ptr<std::string> __buf, const std::string& __format, std::size_t __n, pargs_t... __args) 
    {
        /// Wrapping the function, no need to overcomplicate things.
        __buf = vnformat(__format, __n, __args...);
    };



    ///------------------------------------- @section Wide-strings / Non-unicode strings formatting ------------------------------------------///



    /// @brief Formats a string (with a specified length).
    /// @tparam pargs_t A template for packed arguments (no va_args).
    /// @param __format The string that is going to be formatted.
    /// @param __n The length of characters you want to format.
    /// @param ...__args Virtual packed arguments to format.
    /// @return Returns a formatted string if the length isn't 0, otherwise it returns a empty string.
    template<typename ... pargs_t>
    static std::wstring
    _GLIBCXX_NODISCARD
    wnformat(const std::wstring& __format, std::size_t __n, pargs_t... __args) 
    {
        /// Checking the length of the string.
        if(auto sz = (std::size_t) std::swprintf( nullptr, 0, __format.c_str(), __args...) + 1; sz > 0 && __n != 0)
        {
            /// Initialize a temporary buffer.
            std::unique_ptr<wchar_t[]> buf(new wchar_t[__n]);

            /// Use std::snprintf() and return the string.
            std::swprintf(buf.get(), __n, __format.c_str(), __args...);
            return std::wstring(buf.get(), buf.get() + __n - 1);
        }

        ///  Return a empty string.
        return std::wstring();
    };
    /// @brief Formats a non-unicode string.
    /// @tparam pargs_t A template for packed arguments (no va_args).
    /// @param __format The string that is going to be formatted.
    /// @param ...__args Virtual packed arguments to format.
    /// @return Returns a formatted wstring if the length isn't 0, otherwise it returns a empty wstring.
    template<typename ... pargs_t>
    static std::wstring
    _GLIBCXX_NODISCARD
    wformat(const std::wstring __format, pargs_t... __args) 
    {
        /// Checking the length of the string.
        if(auto sz = (std::size_t) std::swprintf( nullptr, 0, __format.c_str(), __args...) + 1; sz > 0)
        {
            /// Initialize a temporary buffer.
            std::unique_ptr<wchar_t[]> buf(new wchar_t[sz]);

            /// Use std::snprintf() and return the string.
            std::swprintf(buf.get(), sz, __format.c_str(), __args...);
            return std::wstring(buf.get(), buf.get() + sz - 1);
        }

        ///  Return a empty string.
        return std::wstring();
    };
    /// @brief Formats a wstring (with std::optional).
    /// @return Returns a std::optional of the formatted string.
    template<typename ... pargs_t>
    static const std::optional<std::wstring>
    _GLIBCXX_NODISCARD 
    woformat(const std::wstring& __format, pargs_t... __args) 
    {
        /// Call vformat() and check if its 0.
        auto s = vformat(__format, __args...);

        /// If it is not 0 then we make a optional out of it and return it, otherwise we return std::nullopt.
        return s.length() == 0 ? std::make_optional(s) : std::nullopt;
    };
    /// @brief Formats a wstring to a wide string buffer.
    /// @param __buf The buffer to be formatted to.
    template<typename ... pargs_t>
    static void 
    wsformat(std::shared_ptr<std::wstring> __buf, const std::wstring& __format, pargs_t... __args) 
    {
        /// Wrapping the function, no need to overcomplicate things.
        __buf = vformat(__format, __args...);
    };
    /// @brief Formats a wstring to a wide string buffer.
    /// @param __n
    /// @param __buf The buffer to be formatted to.
    template<typename ... pargs_t>
    static void 
    wsnformat(std::shared_ptr<std::wstring> __buf, const std::wstring& __format, std::size_t __n, pargs_t... __args) 
    {
        /// Wrapping the function, no need to overcomplicate things.
        __buf = vnformat(__format, __n, __args...);
    };
};