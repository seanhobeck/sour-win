/**
 *
 *      @author Sean Hobeck
 *       @date 02-25-2023
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
            sdk::dynamic_entity_t*, model_attach_t, std::int32_t, std::int32_t, float);

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
        /*
        * 
    static void sendposition(fpsent *d, packetbuf &q)
    {
        putint(q, N_POS);
        putuint(q, d->clientnum);
        // 3 bits phys state, 1 bit life sequence, 2 bits move, 2 bits strafe
        uchar physstate = d->physstate | ((d->lifesequence&1)<<3) | ((d->move&3)<<4) | ((d->strafe&3)<<6);
        q.put(physstate);
        ivec o = ivec(vec(d->o.x, d->o.y, d->o.z-d->eyeheight).mul(DMF));
        uint vel = min(int(d->vel.magnitude()*DVELF), 0xFFFF), fall = min(int(d->falling.magnitude()*DVELF), 0xFFFF);
        // 3 bits position, 1 bit velocity, 3 bits falling, 1 bit material
        uint flags = 0;
        if(o.x < 0 || o.x > 0xFFFF) flags |= 1<<0;
        if(o.y < 0 || o.y > 0xFFFF) flags |= 1<<1;
        if(o.z < 0 || o.z > 0xFFFF) flags |= 1<<2;
        if(vel > 0xFF) flags |= 1<<3;
        if(fall > 0)
        {
            flags |= 1<<4;
            if(fall > 0xFF) flags |= 1<<5;
            if(d->falling.x || d->falling.y || d->falling.z > 0) flags |= 1<<6;
        }
        if((lookupmaterial(d->feetpos())&MATF_CLIP) == MAT_GAMECLIP) flags |= 1<<7;
        putuint(q, flags);
        loopk(3)
        {
            q.put(o[k]&0xFF);
            q.put((o[k]>>8)&0xFF);
            if(o[k] < 0 || o[k] > 0xFFFF) q.put((o[k]>>16)&0xFF);
        }
        uint dir = (d->yaw < 0 ? 360 + int(d->yaw)%360 : int(d->yaw)%360) + clamp(int(d->pitch+90), 0, 180)*360;
        q.put(dir&0xFF);
        q.put((dir>>8)&0xFF);
        q.put(clamp(int(d->roll+90), 0, 180));
        q.put(vel&0xFF);
        if(vel > 0xFF) q.put((vel>>8)&0xFF);
        float velyaw, velpitch;
        vectoyawpitch(d->vel, velyaw, velpitch);
        uint veldir = (velyaw < 0 ? 360 + int(velyaw)%360 : int(velyaw)%360) + clamp(int(velpitch+90), 0, 180)*360;
        q.put(veldir&0xFF);
        q.put((veldir>>8)&0xFF);
        if(fall > 0)
        {
            q.put(fall&0xFF);
            if(fall > 0xFF) q.put((fall>>8)&0xFF);
            if(d->falling.x || d->falling.y || d->falling.z > 0)
            {
                float fallyaw, fallpitch;
                vectoyawpitch(d->falling, fallyaw, fallpitch);
                uint falldir = (fallyaw < 0 ? 360 + int(fallyaw)%360 : int(fallyaw)%360) + clamp(int(fallpitch+90), 0, 180)*360;
                q.put(falldir&0xFF);
                q.put((falldir>>8)&0xFF);
            }
        }
    }
         *
         */
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