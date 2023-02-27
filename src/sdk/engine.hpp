/**
 *
 *      @author Sean Hobeck
 *       @date 02-27-2023
 *
 **/
#pragma once

 /// @uses: player_t, dynamic_entity_t, phys_entity_t, etc..
#include "entity.hpp"
#include "../util/draw.hpp"
#include "../util/log.hpp"


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
    /// 
    /// @ref: fpsgame/game.h:804
    typedef sdk::dynamic_entity_t* (__fastcall* intersect_t)(const vector_t*, const vector_t*, sdk::player_t*, float*);

    /// Type definition for getclient() & newclient().
    /// 
    /// @ref: fpsgame/game.h:721
    ///       fpsgame/game.h:722  
    typedef sdk::player_t* (__fastcall* client_upd_t)(std::int32_t);

    /// Type definition for damaged().
    /// 
    /// @ref: fpsgame/game.h:740
    typedef void(__fastcall* on_damaged_t)(std::int32_t, sdk::player_t*, sdk::player_t*, bool);

    /// Type definition for killed().
    /// 
    /// @ref: fpsgame/game.h:741
    typedef void(__fastcall* on_killed_t)(sdk::player_t*, sdk::player_t*);

    /// Type definition for  game::gameconnect() & game::gamedisconnect()
    /// 
    /// @ref: shared/igame.h:29, shared/igame.h:33
    typedef void(__fastcall* connect_t)(bool);

    /// @brief Structure Unknown
    struct model_t;

    /// @brief Structure Unknown
    struct model_attach_t 
    {
        const char *m_sz_tag, *m_sz_name;
        std::int32_t m_ianim, m_ibase_time;
        vector_t* m_p_origin;
        model_t* m_p_model;
    };

    /// Type definition for  rendermodel()
    /// 
    /// @ref shared/iengine.h:436
    typedef void(__fastcall* gle_mdl_t)
        (sdk::ent_light_t*, const char*, std::int32_t, const vector_t*, float, float, std::int32_t,
            sdk::dynamic_entity_t*, model_attach_t*, std::int32_t, std::int32_t, float);


    /// Type definition for  isthirdperson()
    /// 
    /// @ref shared/iengine.h:54
    typedef bool(__fastcall* is_thirdperson_t)();

    /// Type definition for  game::allowthirdperson()
    /// 
    /// @ref shared/igame.h:87
    typedef bool(__fastcall* allow_thirdperson_t)(bool);

    /// Type definition for  game::allowthirdperson()
    /// 
    /// @ref shared/igame.h:87
    typedef void(__fastcall* offset_ray_t)(const vector_t*, const vector_t*, std::int32_t, float, vector_t*);

    /// @note: Namespace for net-based engine things.
    namespace net 
    {
        /// @brief Structure for data buffers.
        ///
        /// @ref: shared/tools.h:236
        template <class T>
        struct data_buf_t
        {
            enum
            {
                OVERREAD = 1 << 0,
                OVERWROTE = 1 << 1
            };

            T* buf;
            std::int32_t len, maxlen;
            std::uint8_t flags;

            data_buf_t() : buf(NULL), len(0), maxlen(0), flags(0) {}

            template<class U>
            data_buf_t(T* buf, U maxlen) : buf(buf), len(0), maxlen((int)maxlen), flags(0) {}
        };

        /// Buffer for signed bytes.
        typedef data_buf_t<std::int8_t> i8_buf_t;
        /// Buffer for unsigned bytes.
        typedef data_buf_t<std::uint8_t> u8_buf_t;

        /// ENet packet free callback.
        typedef void(__cdecl* enet_packet_free_callback) (struct _enet_packet_t*);

        /// ENet struct packet.
        typedef struct _enet_packet_t
        {
            size_t referenceCount;
            std::uint32_t flags;
            std::uint8_t* data;
            size_t dataLength;
            enet_packet_free_callback freeCallback; 
            void* userData;
        } enet_packet_t;

        /// @brief Packetbuffer structure.
        ///
        /// @ref: shared/tools.h:337
        struct packet_buf_t : u8_buf_t
        {
            enet_packet_t* packet;
            int growth;
        };

        /// Type definition for sendposition().
        /// 
        /// @ref: fpsgame/game.h:766
        /*
         *
         * void sendposition(fpsent *d, bool reliable)
         * {
         *     if(d->state != CS_ALIVE && d->state != CS_EDITING) return;
         *     packetbuf q(100, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
         *     sendposition(d, q);
         *     sendclientpacket(q.finalize(), 0);
         * }
         *
         */
        typedef void(__fastcall* pos_upd_t)(sdk::player_t*, bool);

        /// Type definition for the static sendposition().
        /// 
        /// @ref: fpsgame/client.cpp:933
        typedef void(__fastcall* static_pos_upd_t)(sdk::player_t*, bool);

        /// @returns Packet information on the packet sent.
        static void*
        get_info_t(packet_buf_t& r_packet) 
        {
            auto npos = r_packet.buf[0];
            return nullptr;
        };
    };
};