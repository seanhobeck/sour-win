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


/// @note: For clarity

#define BASE_PTR void*
#define STDCALL __stdcall
#define FASTCALL __fastcall


/// @brief: sauerbraten SDK namespace.
namespace sdk
{
    /// @brief: Pointer to the base executable.
    static BASE_PTR gp_base = nullptr;

    /// @brief: Pointer to the Opengl & SDL2 modules.
    static BASE_PTR gp_opengl = nullptr; 
    static BASE_PTR gp_sdl2 = nullptr;
    static BASE_PTR gp_sdl2_image = nullptr; 
    static BASE_PTR gp_sdl2_mixer = nullptr;
};

/// @brief: Namespace for all things Hooking.
namespace hk
{
    ///--------------- @section: Original Functions --------------///


    /// Original function for  offsetray.
    static engine::offset_ray_t o_offset_ray;
    /// Original function for  allowthirdperson.
    static engine::allow_thirdperson_t o_allow;
    /// Original function for  is_thirdperson
    static engine::is_thirdperson_t o_is_thirdperson;
    /// Original Model Rendering.
    static engine::gle_mdl_t o_mdl_rend;
    /// Original Disconnect function.
    static engine::connect_t o_game_disconnect;
    /// Original Connect function.
    static engine::connect_t o_game_connect;
    /// Original Processkey function.
    static engine::process_key_t o_process_key;
    /// Original Swapbuffers function.
    static engine::wgl_swapbuffers_t o_swapbuffers;
    /// Original Intersect function.
    static engine::intersect_t o_intersect;


    ///----------------- @section Hooks -------------------///


    /// Game & Client contexts.
    static engine::hctx_t g_cli;


    /// @brief: Hook for wglSwapBuffers.
    static int STDCALL _h_swapbuffers(engine::hdc_t p_hdc, std::uint32_t d_unk)
    {
        /// Context & Hdc and Initialized instance.
        static bool b_initialized = true;
        const auto ctx = wglGetCurrentContext();
        const auto hdc = wglGetCurrentDC();

        /// Setup Lambda
        auto setup = []() -> void
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            int viewport[4];
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
        /// Creating Context Lambda
        auto context = [p_hdc, ctx, hdc]() -> void
        {
            auto cr = wglCreateContext(p_hdc);
            wglMakeCurrent(p_hdc, cr);
            wglMakeCurrent(hdc, ctx);
            g_cli = cr;
        };

        /// First initialization.
        if (b_initialized) {
            context();
            l::log("created render context");
            b_initialized = false;
        };

        /// Start frame & updating the frame size and vm[16].
        wglMakeCurrent(p_hdc, g_cli);
        setup();
        g::p_matrix = reinterpret_cast<float*>((std::uint64_t)sdk::gp_base + 0x32D040);


        /// Updating & Rendering.
        aim::find();
        esp::loop();
        aim::draw_fov();


        /// End frame.
        wglMakeCurrent(hdc, ctx);

        return o_swapbuffers(p_hdc, d_unk);
    };
    /// @brief: Hook for Processkey.
    static void FASTCALL _h_process_key(int _key, bool _down, int _modstate)
    {
        if (_down && _key > 0) {
            if (_key == SDL_KeyCode::SDLK_F5)
                esp::toggle();
            else if (_key == SDL_KeyCode::SDLK_F6)
                aim::toggle();
            else if (_key == SDL_KeyCode::SDLK_F7)
                esp::toggle_tp();
        }

        o_process_key(_key, _down, _modstate);
    };
    /// @brief: Hook for Intersect.
    static sdk::dynamic_entity_t* FASTCALL _h_intersect(const vector_t* p_from, const vector_t* p_to, sdk::player_t* p_player, float* p_dist)
    {
        if (aim::enabled && p_player == g::p_local)
        {
            if (aim::target != nullptr)
                if (!strstr(aim::target->m_sz_team, p_player->m_sz_team))
                    return o_intersect(&p_player->m_new, &aim::target->m_new, p_player, p_dist);
        }

        return o_intersect(p_from, p_to, p_player, p_dist);
    };
    /// @brief: Hook for gameconnect.
    static void FASTCALL _h_game_connect(bool _remote)
    {
        if (_remote)
            l::log("connecting to local match");
        else
            l::log("connecting to match");

        /// reinitializing
        g::p_local = *reinterpret_cast<sdk::player_t**>((std::uint64_t)sdk::gp_base + 0x2A2560);
        g::p_list = *reinterpret_cast<sdk::entity_list**>((std::uint64_t)sdk::gp_base + 0x346C90);
        g::p_matrix = reinterpret_cast<float*>((std::uint64_t)sdk::gp_base + 0x32D040);
        g::p_playercount = reinterpret_cast<int*>((std::uint64_t)sdk::gp_base + 0x346C9C);

        if (g::p_local != nullptr &&
            g::p_list != nullptr &&
            g::p_matrix != nullptr &&
            g::p_playercount != nullptr)
            l::log("reintialized globals");
        else
            l::log("unable to reinit globals");

        o_game_connect(_remote);
    };
    /// @brief: Hook for gamedisconnect.
    static void FASTCALL _h_game_disconnect(bool cleanup)
    {
        l::log("disconnected from match");

        /// Turning off modules.
        esp::enabled = false;
        aim::enabled = false;

        l::log("reloaded modules");

        o_game_disconnect(cleanup);
    };
    /// @brief: Hook for Model Rendering.
    static void FASTCALL _h_mdl_render(sdk::ent_light_t* p_light, const char* sz_mdl, int anim,
        const vector_t* p_origin, float fl_yaw, float fl_pitch, int cull, sdk::dynamic_entity_t* p_entity,
        engine::model_attach_t* p_attach, int base1, int base2, float trans)
    {
        if (p_entity != nullptr)
        {
            if (p_entity != g::p_local) {
                //l::log("rendered player");
                //glDepthFunc(GL_GREATER);
                //glDepthMask(GL_TRUE);
            }
        }

        /// Calling the original function.
        o_mdl_rend(p_light, sz_mdl, anim, p_origin, fl_yaw, fl_pitch, cull, p_entity, p_attach, base1, base2, trans);
    };
    /// @brief: Hook for Allowthirdperson.
    static bool FASTCALL _h_allow_thirdperson(bool _msg)
    {
        if (esp::thirdperson)
            return true;

        return o_allow(_msg);
    };
    /// @brief: Hook for Isthirdperson.
    static bool FASTCALL _h_is_thirdperson()
    {
        if (esp::thirdperson)
            return true;

        return o_is_thirdperson();
    };
    /// @brief: Hook for Offsetray.
    static void FASTCALL _h_offset_ray(const vector_t* p_from, const vector_t* p_to, int spread, float fl_unk, vector_t* p_unk)
    {
        return o_offset_ray(p_from, p_to, 0, fl_unk, p_unk);
    };



    ///--------------- @section: Minhook -----------------///



    /// @brief: Hooking a 64bit function (abs jmp).
    /// @param _src Original func address.
    /// @param _dst Hooked func address.
    /// @param _dst Original func handler.
    /// @return If we could hook.
    static bool hook(void* _src, void* _dst, void** _orig)
    {
        return MH_CreateHook(_src, _dst, _orig) == MH_OK;
    };
};