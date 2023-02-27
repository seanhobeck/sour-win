/**
 *
 *      @author Sean Hobeck
 *       @date 02-20-2023
 *
 **/
#pragma once

/// @uses: kernel32.dll
#include <windows.h>

/// @uses: std::string_view
#include <iostream>
#include <string_view>



/// @note: Create a virtual function with name and VA ARGS.
#define vfunc(ret, name, ...) virtual ret name(__VA_ARGS__) = 0;
/// @note: Create a const virtual function with name and VA ARGS.
#define vfunc_const(ret, name, ...) virtual ret name(__VA_ARGS__) const = 0;
/// @note: Create padding.
#define pad(nm, bytes) std::uint8_t nm [bytes];
/// @note: Create padding with a size.
#define pad_sz(nm, n, type) std::uint8_t nm [n * sizeof(type)];
/// @note: string merging
#define str_merge_impl(a, b) a##b
#define str_merge(a, b) str_merge_impl(a, b)
/// @note: Create padding.
#define __pad(size) str_merge(_pad,__COUNTER__)[size]
/// @note: Defining Member offset.
#define def_offset(type, name, offset) struct { unsigned char __pad(offset); type name; }
/// @note: Defining offset.
#define def(type, name) struct { type name; }

/// @brief Namespace for logging out to our seperate console.
namespace mem
{
    /// @brief Get module information (if it can be found).
    /// @param sz_module The name of the module you are trying to get.
    /// @return The module found, if nothing was found nullptr is returned.
    static void*
        get_module(const std::string_view sz_module) noexcept
    {
        return GetModuleHandleA(sz_module.data());
    };

    /// @brief Get a exported functions address from a certain module.
    /// @tparam T Function typed.
    /// @param sz_function Function name.
    /// @param sz_module  Module name.
    /// @return The typed function address.
    template<typename T> static T
        get_exp_address(const std::string_view sz_function, const std::string_view sz_module) noexcept
    {
        if (void* __m = get_module(sz_module); __m != nullptr)
            return (T) GetProcAddress((HMODULE) __m, sz_function.data());

        return (T) nullptr;
    };
    template<typename T> static T
        get_exp_address(const std::string_view sz_function, void* h_module) noexcept
    {
        return (T) GetProcAddress((HMODULE) h_module, sz_function.data());
    };

    /// @brief Getting a Virtual Function from a thisptr & index.
    /// @param p_this Pointer to base class.
    /// @param _idx Index of the vfunc.
    /// @return The virtual function from the vtable.
    template<typename T = void*> static constexpr T
        get_vfunc(void* p_this, const std::size_t _idx)
    {
        return (*static_cast<T**>(p_this))[_idx];
    };

    /// @brief If a block of memory is readable.
    /// @param p_block The block of memory.
    /// @return If its readable.
    static const bool
        readable(const void* p_block) 
    {
        MEMORY_BASIC_INFORMATION __mbi = { 0 };

        if (::VirtualQuery(p_block, &__mbi, 48ul))
        {
            if (__mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
                return true;

            return !(__mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY));
        }
    };
};