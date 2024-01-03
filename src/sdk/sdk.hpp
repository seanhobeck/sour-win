/**
 *
 *      @author Sean Hobeck
 *       @date 02-27-2023
 *
 **/
#pragma once

/// @uses: h:: [namespace]
#include "../core/hooks.hpp"


/// @brief: sauerbraten SDK namespace.
namespace sdk
{
    /// @brief: Getting the base modules needed for this client.
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
        g::p_playercount = reinterpret_cast<int*>((std::uint64_t)gp_base + 0x346C9C);

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
        MH_Initialize();

        /// Hooking the exported function.
        if (!hk::hook(mem::get_exp_address<engine::wgl_swapbuffers_t>("wglSwapBuffers", gp_opengl),
            hk::_h_swapbuffers, (void**)&hk::o_swapbuffers))
            l::log("unable to hook wgdi.h -> wglSwapBuffers");
        else
            l::log("hooked wgdi.h -> wglSwapBuffers");

        /// Hooking the exported keypress function
        ///
        /// @ref engine/console.cpp:550 ->     void __fastcall processkey(int, bool, int)
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

        /// Hooking the exported rendermodel function.
        /// 
        /// @ref shared/iengine.h:436 -> void __fastcall rendermodel
        ///         (entitylight *light, const char *mdl, int anim, const vec &o, float yaw, 
        ///     float pitch, int cull, dynent *d, modelattach *a, int basetime, int basetime2, float trans);
        if (!hk::hook((engine::gle_mdl_t)((std::uint64_t)gp_base + (std::uint64_t)0x150990),
            hk::_h_mdl_render, (void**)&hk::o_mdl_rend))
            l::log("unable to hook shared/iengine:436 -> rendermodel");
        else
            l::log("hooked shared/iengine:436 -> rendermodel");

        /// Hooking the exported game::allowthirdperson function
        ///
        /// @ref shared/igame.h:87 ->    bool __fastcall game::allow_thirdperson(bool _msg);
        if (!hk::hook((engine::allow_thirdperson_t)((std::uint64_t)gp_base + (std::uint64_t)0x1c63b0),
            hk::_h_allow_thirdperson, (void**)&hk::o_allow))
            l::log("unable to hook shared/igame.h:87 -> allowthirdperson");
        else
            l::log("hooked shared/igame.h:87 -> allowthirdperson");

        /// Hooking the exported game::isthirdperson function
        ///
        /// @ref shared/iengine.h:54 ->    bool __fastcall is_thirdperson();
        if (!hk::hook((engine::is_thirdperson_t)((std::uint64_t)gp_base + (std::uint64_t)0x146e00),
            hk::_h_is_thirdperson, (void**)&hk::o_is_thirdperson))
            l::log("unable to hook shared/iengine.h:54 -> isthirdperson");
        else
            l::log("hooked shared/iengine.h:54 -> isallowthirdperson");

        if (!hk::hook((engine::offset_ray_t)((std::uint64_t)gp_base + (std::uint64_t)0x1d90b0),
            hk::_h_offset_ray, (void**)&hk::o_offset_ray))
            l::log("unable to hook ? -> offsetray");
        else
            l::log("hooked ? -> offsetray");


        /// Enabling all of the hooks.
        MH_EnableHook(MH_ALL_HOOKS);
    };
};