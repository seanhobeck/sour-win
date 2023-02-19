/**
 * 
 *      @author Sean Hobeck
 *       @date 02-15-2023
 * 
 **/
#pragma once

/// @uses: mem:: (namespace)
#include "../util/memory.hpp"

/// @uses: vec, vec2, etc..
#include "geom.hpp"

/// @uses: all enums...
#include "enum.hpp"


/// @brief sauerbraten SDK namespace.
namespace sdk 
{
    /// @brief Class structure for a basic entity.
    class entity_t
    {
    public:

        /// Origin Vector
        vector_t origin;
        /// Padding
        pad_sz(attr, 5, short);
        std::uint8_t type;
        pad(res, 1);
    };

    /// @brief Class structure for a light entity.
    class ent_light_t
    {
    public:

        /// Color, Direction and Time length.
        vector_t color, dir;
        std::int32_t millis;
    };

    /// @brief Class structure for a extended entity.
    class ext_entity_t : public entity_t
    {
    public:

        /// Flags, Light Ent, and the attached.
        std::int32_t m_iflags;
        ent_light_t m_slight;
        ext_entity_t* m_pattached;

        ///------------- Constructor ------------///

        ext_entity_t() {};

        ///----------- Member Functions ----------///

        bool spawned() const { return (m_iflags & EF_SPAWNED) != 0; }
        void setspawned(bool val) { if (val) m_iflags |= EF_SPAWNED; else m_iflags &= ~EF_SPAWNED; }
        void setspawned() { m_iflags |= EF_SPAWNED; }
        void clearspawned() { m_iflags &= ~EF_SPAWNED; }

        bool nopickup() const { return (m_iflags & EF_NOPICKUP) != 0; }
        void setnopickup(bool val) { if (val) m_iflags |= EF_NOPICKUP; else m_iflags &= ~EF_NOPICKUP; }
        void setnopickup() { m_iflags |= EF_NOPICKUP; }
        void clearnopickup() { m_iflags &= ~EF_NOPICKUP; }
    };

    /// @brief Class structure for physics entities.
    class phys_entity_t
    {
    public:

        /// Vectors for origin, velocity, falling vector, delta & new (reset physics entity)
        vector_t m_origin, m_velocity, m_falling,
                m_delta, m_new;
        /// Yaw, Pitch, Roll, Max speed, radius & height.
        float m_flyaw, m_flpitch, m_flroll,
                m_flmaxspeed, m_flradius, m_flheight,
                m_flaboveeye, m_flxrad, m_flyrad, m_flzmarg;
        vector_t m_floor;
        std::uint16_t m_airtime;
        std::uint8_t m_bwater;
        bool m_bjumping;
        std::uint16_t m_move, m_strafe;
        std::uint8_t m_phys, m_state, m_edit, 
                m_type, m_collide, m_blocked;

        
        ///----------------- Constructors ------------------///


        phys_entity_t() { };


        ///-------------- Struct Functions -----------------///


        /// @brief Resetting the entire physics entity.
        void reset() 
        {
            m_new = m_origin;
            m_delta = vector_t(0.f, 0.f, 0.f);
        };

        /// @brief Getting the position of the feet of the entity.
        /// @param fl_offset Additional offset.
        /// @return Position of the feet of the entity.
        vector_t get_feet_origin(float fl_offset = 0.f) 
        {
           return m_origin - vector_t(0.f, m_flheight + fl_offset, 0.f);
        };

        /// @brief Getting the position of the head of the entity.
        /// @param fl_offset Additional offset.
        /// @return Position of the head of the entity.
        vector_t get_head_origin(float fl_offset = 0.f) 
        {
           return m_origin - vector_t(0.f, fl_offset, 0.f);
        };
    };

    /// @brief Structure for animation information.
    struct animation_info_t 
    {
        /// Animation type, frame count, range, and base time.
        std::int32_t m_ianim, m_iframe, m_irange, m_ibasetime;
        /// Animation speed.
        float m_flspeed;
        pad(_vs, 4ul);

        ///------------------- Operators --------------------///

        bool operator==(const animation_info_t& _u) const 
        { 
            return m_iframe == _u.m_iframe && m_irange == _u.m_irange && 
                    (m_ianim & (ANIM_SETTIME|ANIM_DIR)) == (_u.m_ianim & (ANIM_SETTIME|ANIM_DIR)) && 
                    (m_ianim & ANIM_SETTIME || m_ibasetime == _u.m_ibasetime) && m_flspeed == _u.m_flspeed; 
        }
        bool operator!=(const animation_info_t& _u) const 
        {
            return !(*this == _u);
        };
    };

    /// @brief Structure for entity interpolation.
    struct interp_info_t 
    {
        animation_info_t m_previous, m_current;
        std::int32_t m_ilast;
        void* m_last;
    };

    /// Struct declaration.
    struct occlude_query_t;
    /// Struct declaration.
    struct ragdoll_data_t;

    /// @brief Class structure for animated characters or characters that can recieve input.
    class dynamic_entity_t : public phys_entity_t
    {
    public:

        /// Different types of input. (left, right, up, down)
        std::uint8_t m_input[4];
        /// Padding.
        pad_sz(m_light, 1ul, ent_light_t);
        /// Animation Interpolation, Ragdoll pointer, Occlude Query.
        interp_info_t m_animations[3];
        ragdoll_data_t* m_pragdoll;
        occlude_query_t* m_pquery;
        /// Last time it was rendered.
        std::int32_t m_ilastrender;
        /// If the entity is occluded.
        std::uint8_t m_boccluded;

        ///------------ Constructors & Destructors -------------///
        
        dynamic_entity_t() : m_ilastrender(0), m_boccluded(0), m_pquery(nullptr), m_pragdoll(nullptr) 
        {
            reset();
        };

        ~dynamic_entity_t() 
        {

        };
    };

    /// @brief Class structure for player state.
    /// @link: src/fpsgame/game.h:367
    class player_state_t 
    {
    public:

        /// Health, Max Health, 
        std::int32_t m_ihealth, m_imaxhealth,
                m_iarmor, m_iarmor_type, m_iquadmillis,
                m_igunselect, m_igunwait, m_iammo[12ul], 
                m_iaitype, m_iskill;

        ///------------------- Constructors ---------------///

        player_state_t() : m_imaxhealth(100), m_iaitype(AI_NONE), m_iskill(0) { };

        ///---------------- Member Functions -------------///

        /*

        void baseammo(int gun, int k = 2, int scale = 1)
    {
        ammo[gun] = (itemstats[gun-GUN_SG].add*k)/scale;
    }

    void addammo(int gun, int k = 1, int scale = 1)
    {
        itemstat &is = itemstats[gun-GUN_SG];
        ammo[gun] = min(ammo[gun] + (is.add*k)/scale, is.max);
    }

    bool hasmaxammo(int type)
    {
       const itemstat &is = itemstats[type-I_SHELLS];
       return ammo[type-I_SHELLS+GUN_SG]>=is.max;
    }

    bool canpickup(int type)
    {
        if(type<I_SHELLS || type>I_QUAD) return false;
        itemstat &is = itemstats[type-I_SHELLS];
        switch(type)
        {
            case I_BOOST: return maxhealth<is.max || health<maxhealth;
            case I_HEALTH: return health<maxhealth;
            case I_GREENARMOUR:
                // (100h/100g only absorbs 200 damage)
                if(armourtype==A_YELLOW && armour>=100) return false;
            case I_YELLOWARMOUR: return !armourtype || armour<is.max;
            case I_QUAD: return quadmillis<is.max;
            default: return ammo[is.info]<is.max;
        }
    }

    void pickup(int type)
    {
        if(type<I_SHELLS || type>I_QUAD) return;
        itemstat &is = itemstats[type-I_SHELLS];
        switch(type)
        {
            case I_BOOST:
                maxhealth = min(maxhealth+is.info, is.max);
            case I_HEALTH: // boost also adds to health
                health = min(health+is.add, maxhealth);
                break;
            case I_GREENARMOUR:
            case I_YELLOWARMOUR:
                armour = min(armour+is.add, is.max);
                armourtype = is.info;
                break;
            case I_QUAD:
                quadmillis = min(quadmillis+is.add, is.max);
                break;
            default:
                ammo[is.info] = min(ammo[is.info]+is.add, is.max);
                break;
        }
    }

    void respawn()
    {
        maxhealth = 100;
        health = maxhealth;
        armour = 0;
        armourtype = A_BLUE;
        quadmillis = 0;
        gunselect = GUN_PISTOL;
        gunwait = 0;
        loopi(NUMGUNS) ammo[i] = 0;
        ammo[GUN_FIST] = 1;
    }

    void spawnstate(int gamemode)
    {
        if(m_demo)
        {
            gunselect = GUN_FIST;
        }
        else if(m_insta)
        {
            armour = 0;
            health = 1;
            gunselect = GUN_RIFLE;
            ammo[GUN_RIFLE] = 100;
        }
        else if(m_regencapture)
        {
            extern int regenbluearmour;
            if(regenbluearmour)
            {
                armourtype = A_BLUE;
                armour = 25;
            }
            gunselect = GUN_PISTOL;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        }
        else if(m_tactics)
        {
            armourtype = A_GREEN;
            armour = 100;
            ammo[GUN_PISTOL] = 40;
            int spawngun1 = rnd(5)+1, spawngun2;
            gunselect = spawngun1;
            baseammo(spawngun1, m_noitems ? 2 : 1);
            do spawngun2 = rnd(5)+1; while(spawngun1==spawngun2);
            baseammo(spawngun2, m_noitems ? 2 : 1);
            if(m_noitems) ammo[GUN_GL] += 1;
        }
        else if(m_efficiency)
        {
            armourtype = A_GREEN;
            armour = 100;
            loopi(5) baseammo(i+1);
            gunselect = GUN_CG;
            ammo[GUN_CG] /= 2;
        }
        else if(m_ctf || m_collect)
        {
            armourtype = A_BLUE;
            armour = 50;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        }
        else if(m_sp)
        {
            if(m_dmsp) 
            {
                armourtype = A_BLUE;
                armour = 25;
            }
            ammo[GUN_PISTOL] = 80;
            ammo[GUN_GL] = 1;
        }
        else
        {
            armourtype = A_BLUE;
            armour = 25;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        }
    }

    // just subtract damage here, can set death, etc. later in code calling this
    int dodamage(int damage)
    {
        int ad = (damage*(armourtype+1)*25)/100; // let armour absorb when possible
        if(ad>armour) ad = armour;
        armour -= ad;
        damage -= ad;
        health -= damage;
        return damage;
    }

    int hasammo(int gun, int exclude = -1)
    {
        return gun >= 0 && gun <= NUMGUNS && gun != exclude && ammo[gun] > 0;
    }

        */
    };

    /// Sauerbraten classifies this as a "string"
    /// @link: src/shared/tools.h:158-159
    typedef char string_t[260];

    /// @brief Class Structure for all Player Entities.
    class player_t : public dynamic_entity_t, public player_state_t 
    {
    public:

        /// Weight, Client Number, Privledge level, Ping, Last Update, Action & Gun.
        std::int32_t m_iweight, m_iclientn, m_iprivledge,
                m_last_upd, m_plag, m_iping, m_isequence, m_irespawned, 
                m_isuicide,m_last_pain, m_last_action, m_last_gun;
        bool m_battacking;
        std::int32_t m_iattack_sound, m_iattack_chan, m_idle_sound, 
                m_idle_chan, m_last_taunt, m_last_pickup, m_last_pickup_millis, 
                m_last_base, m_last_ammo, m_flagpickup, m_itokens;
        vector_t m_vcollect;
        /// Kills, Flags, Deaths, Shots & Damage.
        std::int32_t m_ikills, m_iflags, m_ideaths, m_totaldamage, m_totalshots;
        pad(m_editptr, 8ul);
        /// Yaw, Pitch, Roll
        float m_dyaw, m_dpitch, m_droll, m_nyaw, m_npitch, m_nroll;
        std::int32_t m_smoothmillis;
        /// Name, Team, Info, Player model number.
        string_t m_sz_name, m_sz_team, m_sz_info;
        std::int32_t m_imodel;
        pad(m_ai, 8ul);
        std::int32_t m_owner, m_lastnode;
        /// Position of the gun muzzle.
        vector_t m_vmuzzle;
    };


    /// @brief Get the name of a animation
    /// @param idx 
    /// @return 
    static std::optional<std::string> 
    get_anim_name(std::int32_t eanim) 
    {
        static const std::vector<std::string> anims = {
            "dead", "dying", "idle",
            "forward", "backward", "left", "right",
            "hold 1", "hold 2", "hold 3", "hold 4", "hold 5", "hold 6", "hold 7",
            "attack 1", "attack 2", "attack 3", "attack 4", "attack 5", "attack 6", "attack 7",
            "pain", "jump", "sink", "swim",
            "edit", "lag", "taunt", "win", "lose",
            "gun idle", "gun shoot",
            "vwep idle", "vwep shoot", "shield", "powerup",
            "mapmodel", "trigger"
        };
        try { return std::make_optional<std::string>(anims[eanim]); }
        catch(...) { return std::nullopt; }
    };
};