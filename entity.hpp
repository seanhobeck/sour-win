/**
 *
 *      @author Sean Hobeck
 *       @date 02-20-2023
 *
 **/
#pragma once

 /// @uses: mem:: (namespace)
#include "memory.hpp"

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
            m_flmaxspeed;
        float m_flradius, m_flheight,
            m_flaboveeye, m_flxrad, m_flyrad, m_flzmarg;
        vector_t m_floor;
        std::uint16_t m_inair;
        std::uint8_t m_water;
        bool m_bjumping;
        std::int8_t m_move, m_strafe;
        std::uint8_t m_phys, m_state, m_edit,
            m_type, m_collide;
        bool m_blocked;



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
            return m_origin - vector_t(0.f, 0.f, m_flheight + fl_offset);
        };

        /// @brief Getting the position of the head of the entity.
        /// @param fl_offset Additional offset.
        /// @return Position of the head of the entity.
        vector_t get_head_origin(float fl_offset = 0.f)
        {
            return m_origin - vector_t(0.f, m_flheight + fl_offset, 0.f);
        };
    };

    /// @brief Structure for animation information.
    struct animation_info_t
    {
        /// Animation type, frame count, range, and base time.
        std::int32_t m_ianim, m_iframe, m_irange, m_ibasetime;
        /// Animation speed.
        float m_flspeed;
        std::uint32_t m_iseed;

        ///------------------- Operators --------------------///

        bool operator==(const animation_info_t& _u) const
        {
            return m_iframe == _u.m_iframe && m_irange == _u.m_irange &&
                (m_ianim & (ANIM_SETTIME | ANIM_DIR)) == (_u.m_ianim & (ANIM_SETTIME | ANIM_DIR)) &&
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
        bool m_left, m_right, m_up, m_down;
        /// Padding.
        ent_light_t m_light;
        /// Animation Interpolation, Ragdoll pointer, Occlude Query.
        interp_info_t m_animations[3];
        ragdoll_data_t* m_pragdoll;
        occlude_query_t* m_pquery;
        /// Last time it was rendered.
        std::int32_t m_ilastrendered;
        std::uint8_t m_uoccluded;

        ///------------ Constructors & Destructors -------------///

        dynamic_entity_t() : m_ilastrendered(0), m_uoccluded(0), m_pquery(nullptr), m_pragdoll(nullptr)
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
    };

    /// @brief Class structure for ai state.
    class ai_state_t
    {
        int type, millis, targtype, target, idle;
        bool override;
    };

    /// @brief Class structure for ai info.
    class ai_info_t
    {
    public:
        std::vector<ai_state_t> state;
        std::vector<std::int32_t> route;
        vector_t target, spot;
        int enemy, enemyseen, enemymillis, weappref, prevnodes[6], targnode, targlast, targtime, targseq,
            lastrun, lasthunt, lastaction, lastcheck, jumpseed, jumprand, blocktime, huntseq, blockseq, lastaimrnd;
        float targyaw, targpitch, views[3], aimrnd[3];
        bool dontmove, becareful, tryreset, trywipe;
    };


    ///------- @section: Player Information --------///


    /// Sauerbraten classifies this as a "string"
    /// @link: src/shared/tools.h:158-159
    typedef char string_t[260];

    /// @brief Class Structure for all Player Entities.
    class player_t : public dynamic_entity_t, public player_state_t
    {
    public:
        /// @brief For all of the data (to be updated.) 
        
        /// Weight, Client Number, Privledge level, Ping, Last Update, Action & Gun.
        std::int32_t m_iweight, m_iclientn, m_iprivledge,
            m_last_upd, m_plag, m_iping, m_isequence, m_irespawned,
            m_isuicide, m_last_pain, m_last_action, m_last_gun;
        bool m_battacking;
        std::int32_t m_iattack_sound, m_iattack_chan, m_idle_sound,
            m_idle_chan, m_last_taunt, m_last_pickup, m_last_pickup_millis,
            m_last_base, m_last_ammo, m_flagpickup, m_itokens;
        vector_t m_vcollect;
        /// Kills, Flags, Deaths, Shots & Damage.
        std::int32_t m_ikills, m_iflags, m_ideaths, m_totaldamage, m_totalshots;
        void* m_editptr;
        /// Yaw, Pitch, Roll
        float m_dyaw, m_dpitch, m_droll, m_nyaw, m_npitch, m_nroll;
        std::int32_t m_smoothmillis;
        /// Name, Team, Info, Player model number.
        string_t m_sz_name, m_sz_team, m_sz_info;
        std::int32_t m_imodel;
        void* m_ai_ptr;
        std::int32_t m_owner, m_lastnode;
        /// Position of the gun muzzle.
        vector_t m_vmuzzle;
        
    public:
        
        ///-------------- Clean Functions ----------------///

        bool is_valid() { return !IsBadReadPtr(this) && uintptr_t(this) && this != nullptr && this->m_ihealth > 0; }
    };

    /// @brief: Class for the entity list.
    class entity_list 
    {
    private:
        /// @note: List of entities.
        player_t* entities[33];

    public:

        /// Default constructor.
        entity_list() = default;

        /// @note: Gets a entity at the index, otherwise it throws std::out_of_range exception.
        player_t* get_entity(const std::size_t idx) 
        {
            return entities[idx];
        };
    };


    /// @brief Filler class (two vec4s).
    struct dual_t 
    {
        /// The two quaternions (vec4s).
        quatern_t a, b;

        dual_t() = default;
    };

    /// @brief Structure for information on Bones.
    class bone_info_t 
    {
    public:
        /// Name of the bone.
        string_t m_sz_name;
        /// Bone index information.
        std::int32_t m_iparent, m_ichild, m_inext, m_igroup, 
            m_ischeduled, m_interpindex, m_interp_parent, 
            m_iragdoll_index, m_index;
        /// Scale, offset, min, max
        float m_flscale, m_floffset, m_flmin, m_flmax;

        /// Base Quaternion and Inverse Base Quaternion
        dual_t m_qbase, m_qinv_base;


        ///------ Constructors -------///


         /*
        boneinfo_t() : m_sz_name(0), m_iparent(-1), m_ichild(-1), 
            m_inext(-1), m_igroup(std::numeric_limits<std::int32_t>::max()), 
            m_ischeduled(-1), m_interpindex(-1), m_interpparent(-1), 
            m_iragdollindex(-1), m_index(-1), m_flscale(0), m_floffset(0), 
            m_flmin(0), m_flmax(0) { /* Do nothing. * };*/
    };



    ///--------- @section: SDK functions ----------///


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
        catch (...) { return std::nullopt; }
    };
};


/// @brief Namespace for globals.
namespace g
{
    /// @note: Pointer to the local player (us).
    static sdk::player_t* p_local;

    /// @note: Pointer to the entity list.
    static sdk::entity_list* p_list;

    /// @note: Pointer to the player count.
    static std::int32_t* p_playercount;
};