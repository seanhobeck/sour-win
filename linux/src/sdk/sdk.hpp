/**
 * 
 *      @author Sean Hobeck
 *       @date 02-15-2023
 * 
 **/
#pragma once

/// Defining your sour version.
#define SOUR_VERSION "v1.01"

/// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "entity.hpp"

/// @uses: SDL2, mixer & image, zlib, enet, opengl32
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <zlib.h>


/// @brief Namespace for all things Hooking.
namespace hk 
{
    /// @brief Protecting a address with multiple page sizes.
    /// @param _addr Address to protect.
    /// @param _sz Size to protect.
    /// @param _prot Protection level.
    /// @return mprotect() return.
    static std::int32_t 
    protect(std::int64_t _addr, std::size_t _sz, std::int32_t _prot) 
    {
        return mprotect((void*)(_addr - (_addr % sysconf(_SC_PAGE_SIZE))), _sz, _prot);
    };

    /// @brief Hooking a 64bit function (abs jmp).
    /// @param _src Original func address.
    /// @param _dst Hooked func address.
    /// @param _sz Size of bytes copied (2 + 8 + 2)
    /// @return If we could hook.
    static bool
    hook(std::uint64_t _src, std::uint64_t _dst) 
    {
        /// protect with, read, write, exec.
        if (protect(_src, 13ul, 4 | 2 | 1) != 0)
            return false;
        
        /// Hook using Absolute Jump
        ///                     mov         (std::int64_t) $[dst], r11           |    jmp %r11
        std::uint8_t* _p = (std::uint8_t*)((void*)_src);
        *_p = 0x49;
        *(_p + 1) = 0xBB;
        *((std::uint64_t*)_p + 2) = _dst;
        *(_p + 10) = 0x41;
        *(_p + 11) = 0xFF;
        *(_p + 12) = 0xE3;
        return true;
    };


    ///----------------- @section Hooks -------------------///

    /// For clarity.
    typedef _XDisplay* x11_display;

    /// Type definition for swapbuffers.
    typedef void(*glx_swapbuffers_t)(x11_display, std::uint64_t);

    /// Original Swapbuffers function.
    static glx_swapbuffers_t o_swapbuffers;

    /// @brief Hook for glxSwapBuffers.
    /// @param p_display Pointer to the X11 Display.
    /// @param q_drawable GLXDrawable.
    static void
    _h_swapbuffers(x11_display p_display, std::uint64_t q_drawable) 
    {
        std::cout << "render called" << std::endl;
        o_swapbuffers(p_display, q_drawable);
    };
};


/// @brief sauerbraten SDK namespace.
namespace sdk 
{
    /// @brief Pointer to the base executable.
    static void* gp_base = nullptr;

    /// @brief Pointer to the Opengl & SDL2 modules.
    static void* gp_opengl = nullptr, *gp_sdl2 = nullptr, 
        *gp_sdl2_image = nullptr, *gp_sdl2_mixer = nullptr, 
        *gp_x11 = nullptr, *gp_glx = nullptr;

    
    /// @brief Getting the base modules needed for this client.
    static void
    setup()
    {
        /// Get the base executable pointer.
        //gp_base = mem::get_module(0);

        ///----------- @ref: ldd bin_unix/linux_64_client -----------///
        /**
         * 
         * 
         * 
         * 
         * 
         * 
         * 
         **/
        /// @address: 0x00007f8631a09000
        gp_opengl = mem::get_module("libGL.so.1"); 
        /// @address: 0x00007f86310e3000  
        gp_sdl2 = mem::get_module("libSDL2-2.0.so.0");
        /// @address: 0x00007f8631ace000
        gp_sdl2_image = mem::get_module("libSDL2_image-2.0.so.0"); 
        /// @address: 0x00007f8631aaa000  
        gp_sdl2_mixer = mem::get_module("libSDL2_mixer-2.0.so.0");
        /// @address: 0x00007f86312b8000  
        gp_x11 = mem::get_module("libX11.so.6");
        /// @address: 0x00007f5afac17000
        gp_glx = mem::get_module("libGLX.so.0");

        ///------------- Printing all of the modules ---------------///

        #ifndef _DEBUG
        __gnu_cxx::v23::println("[sour-%s]: opengl->%p", SOUR_VERSION, gp_opengl);
        __gnu_cxx::v23::println("[sour-%s]: sdl2->%p", SOUR_VERSION, gp_sdl2);
        __gnu_cxx::v23::println("[sour-%s]: sdl2_image->%p", SOUR_VERSION, gp_sdl2_image);
        __gnu_cxx::v23::println("[sour-%s]: sdl2_mixer->%p", SOUR_VERSION, gp_sdl2_mixer);
        __gnu_cxx::v23::println("[sour-%s]: x11->%p", SOUR_VERSION, gp_x11);
        __gnu_cxx::v23::println("[sour-%s]: glx->%p", SOUR_VERSION, gp_glx);
        #endif

        ///-------------- Initializing all of the Hooks -------------////

        /// Hooking the exported function.
        hk::o_swapbuffers = mem::get_exp_address<hk::glx_swapbuffers_t>("glXSwapBuffers", gp_opengl);
        hk::hook((std::uint64_t) &hk::o_swapbuffers, (std::uint64_t) &hk::_h_swapbuffers);
    };
};