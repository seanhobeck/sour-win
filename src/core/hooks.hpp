/**
 *
 *      @author Sean Hobeck
 *       @date 02-27-2023
 *
 **/
#pragma once

/// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "../features/esp.hpp"
#include "../features/aim.hpp"

/// @uses: hooking engine functions.
#include "../sdk/engine.hpp"

/// @uses: log:: [namespace], config:: [namespace]
#include "../util/config.hpp"

/// @uses:: MH_Initialize
#include "../dependencies/minhook/minhook.h"
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
    _h_swapbuffers(engine::hdc_t p_hdc, std::uint32_t d_unk);



    /// Original Processkey function.
    static engine::process_key_t o_process_key;
    /// @brief Hook for Processkey.
    /// @param _key Keysym pressed.
    /// @param _down If the key was pressed down.
    /// @param _modstate ?
    static void __fastcall
    _h_process_key(std::int32_t _key, bool _down, std::int32_t _modstate);



    /// Original Intersect function.
    static engine::intersect_t o_intersect;
    /// @brief Hook for Intersect.
    /// @param p_from From vector.
    /// @param p_to To vector.
    /// @param p_player Player to hit.
    /// @param p_dist Best distance.
    static sdk::dynamic_entity_t* __fastcall
    _h_intersect(const vector_t* p_from, const vector_t* p_to, sdk::player_t* p_player, float* p_dist);



    /// Original Connect function.
    static engine::connect_t o_game_connect;
    /// @brief Hook for gameconnect.
    static void __fastcall
    _h_game_connect(bool _remote);

    /// Original Disconnect function.
    static engine::connect_t o_game_disconnect;
    /// @brief Hook for gamedisconnect.
    static void __fastcall
    _h_game_disconnect(bool cleanup);



    /// Original Model Rendering.
    static engine::gle_mdl_t o_mdl_rend;
    /// @brief Hook for Model Rendering.
    static void __fastcall
    _h_mdl_render(sdk::ent_light_t* p_light, const char* sz_mdl, std::int32_t anim,
        const vector_t* p_origin, float fl_yaw, float fl_pitch, std::int32_t cull,
        sdk::dynamic_entity_t* p_entity, engine::model_attach_t* p_attach, std::int32_t base1, std::int32_t base2, float trans);


    
    /// Original function for  allowthirdperson.
    static engine::allow_thirdperson_t o_allow;
    /// Original function for  is_thirdperson.
    static engine::is_thirdperson_t o_is_thirdperson;
    /// @brief Hook for Allowthirdperson.
    static bool __fastcall
    _h_allow_thirdperson(bool _msg);
    /// @brief Hook for Isthirdperson.
    static bool __fastcall
    _h_is_thirdperson();



    /// Original function for  offsetray.
    static engine::offset_ray_t o_offset_ray;
    /// @brief Hook for Offsetray.
    static void __fastcall
    _h_offset_ray(const vector_t* p_from, const vector_t* p_to, std::int32_t spread, float fl_unk, vector_t* p_unk);



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