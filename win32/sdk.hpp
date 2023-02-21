/**
 *
 *      @author Sean Hobeck
 *       @date 02-20-2023
 *
 **/
#pragma once

/// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "entity.hpp"

/// @uses: log:: [namespace]
#include "log.hpp"

/// @uses:: MH_Initialize
#include "dependencies/minhook.h"

/// @uses: opengl32, SDL2, tlhelp32
#pragma comment(lib, "OpenGL32.lib")
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <intrin.h>

/// @brief Namespace for globals.
namespace g 
{
    /// @note: Pointer to the local player (us).
    static sdk::player_t* p_local;

    /// @note: Pointer to the entity list.
    static sdk::entity_list* p_list;

    /// @note: Pointer to the view matrix.
    static float* p_matrix;
};


/// @brief Namespace for all things Hooking.
namespace hk
{
    ///----------------- @section Hooks -------------------///


    typedef HDC hdc_t;
    typedef HGLRC hctx_t;
    /// Type definition for swapbuffers.
    typedef std::int32_t(__stdcall *wgl_swapbuffers_t)(hdc_t, std::uint32_t);


    /// Game & Client contexts.
    static hctx_t g_game, g_cli;
    /// Create the Client Contexts.
    static void create_context(const hdc_t p_hdc)
    {
        g_game = wglGetCurrentContext();
        g_cli = wglCreateContext(p_hdc);
        wglMakeCurrent(p_hdc, g_cli);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        std::int32_t m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);
        glOrtho(0.0, m_viewport[2], m_viewport[3], 0.0, 1.0, -1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0, 0, 0, 1.0);
    };
    /// Original Swapbuffers function.
    static wgl_swapbuffers_t o_swapbuffers;
    /// @brief Hook for wglSwapBuffers.
    /// @param p_hdc HDC pointer.
    /// @param q_unk Unknown.
    static std::int32_t __stdcall
    _h_swapbuffers(hdc_t p_hdc, std::uint32_t d_unk)
    {
        static bool cr_ctx = true;

        /// On first run make client context.
        if (cr_ctx) {
            create_context(p_hdc);
            l::log("created render context");
            cr_ctx = false;
        };

        /// Start frame
        wglMakeCurrent(p_hdc, g_cli);

        
        /// Render whatever...


        /// End frame.
        wglMakeCurrent(p_hdc, g_game);

        return o_swapbuffers(p_hdc, d_unk);
    };



    typedef SDL_Event* sdl_event_t;
    /// Type definition for sdl_pollevent.
    typedef std::int32_t(__cdecl* sdl_pollevent_t)(sdl_event_t);

    /// Original Pollevent function.
    static sdl_pollevent_t o_pollevent;
    /// @brief Hook for SDL_PollEvent.
    /// @param p_event The event to be polled.
    static std::int32_t __cdecl
    _h_pollevent(sdl_event_t p_event) 
    {
        if (p_event->type == SDL_EventType::SDL_KEYDOWN)
            std::cout << "Key Pressed Down: " << p_event->key.keysym.sym << std::endl;

        return o_pollevent(p_event);
    };


    ///--------------- @section: Minhook -----------------///


    /// @brief Hooking a 64bit function (abs jmp).
    /// @param _src Original func address.
    /// @param _dst Hooked func address.
    /// @param _dst Original func handler.
    /// @return If we could hook.
    static bool hook(void* _src, void* _dst, void** _orig)
    {
        return MH_CreateHook(_src, _dst, _orig) == MH_OK;
    };

    /// @brief Initializing Minhook.
    static void initialize()
    {
        MH_Initialize();
    };

    /// @brief Enabling all of the hooks.
    static void enable() 
    {
        MH_EnableHook(MH_ALL_HOOKS);
    };
};


/// @brief sauerbraten SDK namespace.
namespace sdk
{
    /// @brief Pointer to the base executable.
    static void* gp_base = nullptr;

    /// @brief Pointer to the Opengl & SDL2 modules.
    static void* gp_opengl = nullptr, * gp_sdl2 = nullptr,
        * gp_sdl2_image = nullptr, * gp_sdl2_mixer = nullptr;


    /// @brief Getting the base modules needed for this client.
    static void setup()
    {
        /// Creating the log console.
        l::create("sour-client");

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
         /// @note: Non-direct import.
        gp_base = GetModuleHandleA(0);
        gp_opengl = mem::get_module("opengl32.dll");

        /// @note: Direct imports.
        gp_sdl2 = mem::get_module("SDL2.dll");
        gp_sdl2_image = mem::get_module("SDL2_image.dll");
        gp_sdl2_mixer = mem::get_module("SDL2_mixer.dll");

        l::log("initialized modules");

        ///----------------- Initializing Globals --------------------///

        g::p_local = (sdk::player_t*)((std::uint64_t)gp_base + (std::uint64_t)0x3472D0);
        g::p_list = (sdk::entity_list*)((std::uint64_t)gp_base + (std::uint64_t)0x346C90);
        g::p_matrix = (float*)((std::uint64_t)gp_base + (std::uint64_t)0x32D040);

        
        if (g::p_local != nullptr && g::p_list != nullptr)
            l::log("intialized globals");
        else
            l::log("improper offsets");

        ///-------------- Initializing all of the Hooks -------------////


        /// Initializing Minhook
        hk::initialize();

        /// Hooking the exported function.
        if (!hk::hook(mem::get_exp_address<hk::wgl_swapbuffers_t>("wglSwapBuffers", gp_opengl),
            hk::_h_swapbuffers, (void**)&hk::o_swapbuffers))
            l::log("unable to hook wglSwapBuffers");
        else
            l::log("hooked wglSwapBuffers");

        /// Hooking the exported function.
        if (!hk::hook(mem::get_exp_address<hk::sdl_pollevent_t>("SDL_PollEvent", gp_sdl2),
            hk::_h_pollevent, (void**)&hk::o_pollevent))
            l::log("unable to hook SDL_PollEvent");
        else
            l::log("hooked SDL_PollEvent");

        /// Enabling all of the hooks.
        hk::enable();
    };
};