/**
 *
 *      @author Sean Hobeck
 *       @date 02-24-2023
 *
 **/
#pragma once

 /// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "entity.hpp"
#include "draw.hpp"
#include "log.hpp"


/// @note: Namespace for all things related to the engine.
namespace engine 
{
    typedef HDC hdc_t;
    typedef HGLRC hctx_t;

    /// Type definition for swapbuffers().
    typedef std::int32_t(__stdcall* wgl_swapbuffers_t)(hdc_t, std::uint32_t);

    /// Type definition for process_key().
    typedef void(__fastcall* process_key_t)(std::int32_t, bool, std::int32_t);

    /// Type definition for intersectclosest().
    /// @ref: fpsgame/game.h:804
    typedef sdk::dynamic_entity_t* (__fastcall* intersect_t)(const vector_t*, const vector_t*, sdk::player_t*, float*);

    /// Type definition for getclient() & newclient().
    /// @ref: fpsgame/game.h:721
    ///       fpsgame/game.h:722  
    typedef sdk::player_t* (__fastcall* client_upd_t)(std::int32_t);

    /// Type definition for damaged().
    /// @ref: fpsgame/game.h:740
    typedef void(__fastcall* on_damaged_t)(std::int32_t, sdk::player_t*, sdk::player_t*, bool);

    /// Type definition for killed().
    /// @ref: fpsgame/game.h:741
    typedef void(__fastcall* on_killed_t)(sdk::player_t*, sdk::player_t*);
};