/**
 *
 *      @author Sean Hobeck
 *       @date 02-25-2023
 *
 **/
#pragma once

/// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "esp.hpp"
#include "aim.hpp"

/// @uses: hooking engine functions.
#include "engine.h"

/// @uses: log:: [namespace], config:: [namespace]
#include "config.hpp"

/// @uses:: MH_Initialize
#include "dependencies/minhook.h"
#include <format>


/// @brief sauerbraten SDK namespace.
namespace sdk 
{
    /// @brief Pointer to the base executable.
    static void* gp_base = nullptr;

    /// @brief Pointer to the Opengl & SDL2 modules.
    static void* gp_opengl = nullptr, * gp_sdl2 = nullptr,
        * gp_sdl2_image = nullptr, * gp_sdl2_mixer = nullptr;
};

/// @brief Namespace for all things Hooking.
namespace hk
{
    ///----------------- @section Hooks -------------------///


    /// Game & Client contexts.
    static engine::hctx_t g_cli;
    /// Original Swapbuffers function.
    static engine::wgl_swapbuffers_t o_swapbuffers;
    /// @brief Hook for wglSwapBuffers.
    /// @param p_hdc HDC pointer.
    /// @param q_unk Unknown.
    static std::int32_t __stdcall
    _h_swapbuffers(engine::hdc_t p_hdc, std::uint32_t d_unk)
    {
        static bool cr_ctx = true;

        /// Context & Hdc
        const auto ctx = wglGetCurrentContext();
        const auto hdc = wglGetCurrentDC();

        
        ///- Lambda Functions -///

        auto setup = []() -> void 
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            std::int32_t viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            glViewport(0, 0, viewport[2], viewport[3]);
            g::p_width = viewport[2];
            g::p_height = viewport[3];

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_DEPTH_TEST);
        };
        auto context = [p_hdc, ctx, hdc]() -> void
        {
            auto cr = wglCreateContext(p_hdc);
            wglMakeCurrent(p_hdc, cr);
            wglMakeCurrent(hdc, ctx);
            g_cli = cr;
        };


        /// On first run make client context.
        if (cr_ctx) {
            context();
            l::log("created render context");
            cr_ctx = false;
        };

        /// Start frame & Updating the frame size and View Matrix.
        wglMakeCurrent(p_hdc, g_cli);
        setup();
        g::p_matrix = reinterpret_cast<float*>((std::uint64_t)sdk::gp_base + 0x32D040);
        

        /// Updating
        if (legit::gb_t) 
            legit::find();

        /// Render whatever...
        esp::render();
        legit::fov();


        /// End frame.
        wglMakeCurrent(hdc, ctx);

        return o_swapbuffers(p_hdc, d_unk);
    };



    /// Original Processkey function.
    static engine::process_key_t o_process_key;
    /// @brief Hook for Processkey.
    /// @param _key Keysym pressed.
    /// @param _down If the key was pressed down.
    /// @param _modstate ?
    static void __fastcall
    _h_process_key(std::int32_t _key, bool _down, std::int32_t _modstate) 
    {
        if (_down && _key > 0) {
            if (_key == SDL_KeyCode::SDLK_F5)
                esp::toggle();
            else if (_key == SDL_KeyCode::SDLK_F6)
                legit::toggle();
            //else if (_key == SDL_KeyCode::SDLK_F7)
            //    rage::toggle();
        }

        o_process_key(_key, _down, _modstate);
    };



    /// Original Intersect function.
    static engine::intersect_t o_intersect;
    /// @brief Hook for Intersect.
    /// @param p_from From vector.
    /// @param p_to To vector.
    /// @param p_player Player to hit.
    /// @param p_dist Best distance.
    static sdk::dynamic_entity_t* __fastcall
    _h_intersect(const vector_t* p_from, const vector_t* p_to, sdk::player_t* p_player, float* p_dist)
    {
        if (legit::gb_t && p_player == g::p_local)
        {
            auto trgt = legit::nearest;
            
            if (trgt != nullptr)
                if (!strstr(trgt->m_sz_team, p_player->m_sz_team))
                    return o_intersect(&p_player->m_new, &trgt->m_new, p_player, p_dist);
        }

        return o_intersect(p_from, p_to, p_player, p_dist);
    };

    /// Original Connect function.
    static engine::connect_t o_game_connect;
    /// @brief Hook for gameconnect.
    static void __fastcall
    _h_game_connect(bool _remote)
    {
        if (_remote)
            l::log("connecting to local match");
        else
            l::log("connecting to match");

        /// reinitializing
        g::p_local = *reinterpret_cast<sdk::player_t**>((std::uint64_t)sdk::gp_base + 0x2A2560);
        g::p_list = *reinterpret_cast<sdk::entity_list**>((std::uint64_t)sdk::gp_base + 0x346C90);
        g::p_matrix = reinterpret_cast<float*>((std::uint64_t)sdk::gp_base + 0x32D040);
        g::p_playercount = reinterpret_cast<std::int32_t*>((std::uint64_t)sdk::gp_base + 0x346C9C);

        if (g::p_local != nullptr &&
            g::p_list != nullptr &&
            g::p_matrix != nullptr &&
            g::p_playercount != nullptr)
            l::log("reintialized globals");
        else
            l::log("unable to reinit globals");

        o_game_connect(_remote);
    };

    /// Original Disconnect function.
    static engine::connect_t o_game_disconnect;
    /// @brief Hook for gamedisconnect.
    static void __fastcall
    _h_game_disconnect(bool cleanup)
    {
        l::log("disconnected from match");

        /// turn off all modules
        esp::gb_t = false;
        legit::gb_t = false;

        l::log("reloaded modules");

        o_game_disconnect(cleanup);
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
    /// @brief Getting the base modules needed for this client.
    static void setup()
    {
        /// Creating the log console.
        l::create("sour-client");

        ///----------- @ref: ldd bin_unix/linux_64_client -----------///
        /**
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

        /// Initializing the configuration.
        config::init();

        ///----------------- Initializing Globals --------------------///

        g::p_local = *reinterpret_cast<sdk::player_t**>((std::uint64_t)gp_base + 0x2A2560);
        g::p_list = *reinterpret_cast<sdk::entity_list**>((std::uint64_t)gp_base + 0x346C90);
        g::p_matrix = reinterpret_cast<float*>((std::uint64_t)gp_base + 0x32D040);
        g::p_playercount = reinterpret_cast<std::int32_t*>((std::uint64_t)gp_base + 0x346C9C);
        
        /// Sanity checking.
        if (g::p_local != nullptr && 
            g::p_list != nullptr && 
            g::p_matrix != nullptr && 
            g::p_playercount != nullptr)
            l::log("intialized globals");
        else
            l::log("improper offsets");

        ///-------------- Initializing all of the Hooks -------------////


        /// Initializing Minhook
        hk::initialize();




        /// Hooking the exported function.
        if (!hk::hook(mem::get_exp_address<engine::wgl_swapbuffers_t>("wglSwapBuffers", gp_opengl),
            hk::_h_swapbuffers, (void**)&hk::o_swapbuffers))
            l::log("unable to hook wgdi.h -> wglSwapBuffers");
        else
            l::log("hooked wgdi.h -> wglSwapBuffers");

        /// Hooking the exported keypress function
        ///
        /// @ref engine/console.cpp:550 ->     void __fastcall processkey(std::int32_t, bool, std::int32_t)
        ///
        if (!hk::hook((engine::process_key_t)((std::uint64_t)gp_base + (std::uint64_t)0x1a0260),
            hk::_h_process_key, (void**)&hk::o_process_key))
            l::log("unable to hook engine/console.cpp:550 -> processkey");
        else
            l::log("hooked engine/console.cpp:550 -> processkey");

        /// Hooking the exported intersect function
        ///
        /// @ref fpsgame/weapon.cpp:714 ->    sdk::dynamic_entity_t* __fastcall intersectclosest(const vector_t&, const vector_t&, sdk::player_t*, float&)
        ///
        if (!hk::hook((engine::intersect_t)((std::uint64_t)gp_base + (std::uint64_t)0x1db2a0),
            hk::_h_intersect, (void**)&hk::o_intersect))
            l::log("unable to hook fpsgame/weapon.cpp:714 -> intersectclosest");
        else
            l::log("hooked fpsgame/weapon.cpp:714 -> intersectclosest");

        /// Hooking the exported game:gameconnect function
        ///
        /// @ref shared/igame.h:33 ->    void __fastcall game::gameconnect(bool _remote);
        if (!hk::hook((engine::connect_t)((std::uint64_t)gp_base + (std::uint64_t)0x1e7750),
            hk::_h_game_connect, (void**)&hk::o_game_connect))
            l::log("unable to hook shared/igame.h:33 -> gameconnect");
        else
            l::log("hooked shared/igame.h:33 -> gameconnect");

        /// Hooking the exported game:gamedisconnect function
        ///
        /// @ref shared/igame.h:29 ->    void __fastcall game::gameconnect(bool _remote);
        if (!hk::hook((engine::connect_t)((std::uint64_t)gp_base + (std::uint64_t)0x1e75f0),
            hk::_h_game_disconnect, (void**)&hk::o_game_disconnect))
            l::log("unable to hook shared/igame.h:29 -> gamedisconnect");
        else
            l::log("hooked shared/igame.h:29 -> gamedisconnect");




        /// Enabling all of the hooks.
        hk::enable();
    };
};