#include "../sdk/sdk.hpp"


///--------------------------- @section: Hooks -----------------------------///


sdk::dynamic_entity_t* __fastcall hk::_h_intersect(const vector_t* p_from, const vector_t* p_to, sdk::player_t* p_player, float* p_dist)
{
    if (legit::gb_t && p_player == g::p_local)
    {
        auto trgt = legit::nearest;

        if (trgt != nullptr)
            if (!strstr(trgt->m_sz_team, p_player->m_sz_team))
                return o_intersect(&p_player->m_new, &trgt->m_new, p_player, p_dist);
    }

    return o_intersect(p_from, p_to, p_player, p_dist);
}

std::int32_t __stdcall hk::_h_swapbuffers(engine::hdc_t p_hdc, std::uint32_t d_unk)
{
    static bool b_initialized = true;

    /// Context & Hdc
    const auto ctx = wglGetCurrentContext();
    const auto hdc = wglGetCurrentDC();

    /// Setup Lambda
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

bool __fastcall hk::_h_allow_thirdperson(bool _msg)
{
    if (esp::thirdperson)
        return true;

    return o_allow(_msg);
}

bool __fastcall hk::_h_is_thirdperson()
{
    if (esp::thirdperson)
        return true;

    return o_is_thirdperson();
};

void __fastcall hk::_h_process_key(std::int32_t _key, bool _down, std::int32_t _modstate)
{
    if (_down && _key > 0) {
        if (_key == SDL_KeyCode::SDLK_F5)
            esp::toggle();
        else if (_key == SDL_KeyCode::SDLK_F6)
            legit::toggle();
        else if (_key == SDL_KeyCode::SDLK_F7)
            esp::toggle_thirdperson();
    }

    o_process_key(_key, _down, _modstate);
};

void __fastcall hk::_h_game_connect(bool _remote)
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
}

void __fastcall hk::_h_game_disconnect(bool cleanup)
{
    l::log("disconnected from match");

    /// turn off all modules
    esp::gb_t = false;
    legit::gb_t = false;

    l::log("reloaded modules");

    o_game_disconnect(cleanup);
};

void __fastcall hk::_h_mdl_render(sdk::ent_light_t* p_light, const char* sz_mdl, std::int32_t anim, const vector_t* p_origin, float fl_yaw, float fl_pitch, std::int32_t cull, sdk::dynamic_entity_t* p_entity, engine::model_attach_t* p_attach, std::int32_t base1, std::int32_t base2, float trans)
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

void __fastcall hk::_h_offset_ray(const vector_t* p_from, const vector_t* p_to, std::int32_t spread, float fl_unk, vector_t* p_unk)
{
    return o_offset_ray(p_from, p_to, 0, fl_unk, p_unk);
};