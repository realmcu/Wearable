/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.6 at Fri Jun 10 15:10:20 2022. */

#ifndef PB_WEAR_MEDIA_PB_H_INCLUDED
#define PB_WEAR_MEDIA_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _Media_MediaID
{
    Media_MediaID_WEAR_REQUEST = 0,
    Media_MediaID_SYNC_PLAYER_INFO = 1,
    Media_MediaID_CONTROL_PLAYER = 2
} Media_MediaID;
#define _Media_MediaID_MIN Media_MediaID_WEAR_REQUEST
#define _Media_MediaID_MAX Media_MediaID_CONTROL_PLAYER
#define _Media_MediaID_ARRAYSIZE ((Media_MediaID)(Media_MediaID_CONTROL_PLAYER+1))

typedef enum _PlayerInfo_State
{
    PlayerInfo_State_NONE = 0,
    PlayerInfo_State_PLAYING = 1,
    PlayerInfo_State_PAUSE = 2,
    PlayerInfo_State_STOP = 3,
    PlayerInfo_State_NO_PERMISSION = 10
} PlayerInfo_State;
#define _PlayerInfo_State_MIN PlayerInfo_State_NONE
#define _PlayerInfo_State_MAX PlayerInfo_State_NO_PERMISSION
#define _PlayerInfo_State_ARRAYSIZE ((PlayerInfo_State)(PlayerInfo_State_NO_PERMISSION+1))

typedef enum _PlayerControl_Command
{
    PlayerControl_Command_PLAY = 0,
    PlayerControl_Command_PAUSE = 1,
    PlayerControl_Command_STOP = 2,
    PlayerControl_Command_PREV = 3,
    PlayerControl_Command_NEXT = 4,
    PlayerControl_Command_ADJUST_VOLUME = 5
} PlayerControl_Command;
#define _PlayerControl_Command_MIN PlayerControl_Command_PLAY
#define _PlayerControl_Command_MAX PlayerControl_Command_ADJUST_VOLUME
#define _PlayerControl_Command_ARRAYSIZE ((PlayerControl_Command)(PlayerControl_Command_ADJUST_VOLUME+1))

/* Struct definitions */
typedef struct _PlayerControl
{
    PlayerControl_Command command;
    bool has_volume;
    uint8_t volume;
    /* @@protoc_insertion_point(struct:PlayerControl) */
} PlayerControl;

typedef struct _PlayerInfo
{
    PlayerInfo_State state;
    bool has_volume;
    uint8_t volume;
    char *player_name;
    char *song_title;
    char *song_artist;
    bool has_current_position;
    uint16_t current_position;
    bool has_duration;
    uint16_t duration;
    /* @@protoc_insertion_point(struct:PlayerInfo) */
} PlayerInfo;

typedef struct _Media
{
    pb_size_t which_payload;
    union
    {
        PlayerInfo player_info;
        PlayerControl player_control;
    } payload;
    /* @@protoc_insertion_point(struct:Media) */
} Media;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Media_init_default                       {0, {PlayerInfo_init_default}}
#define PlayerInfo_init_default                  {_PlayerInfo_State_MIN, false, 0, NULL, NULL, NULL, false, 0, false, 0}
#define PlayerControl_init_default               {_PlayerControl_Command_MIN, false, 0}
#define Media_init_zero                          {0, {PlayerInfo_init_zero}}
#define PlayerInfo_init_zero                     {_PlayerInfo_State_MIN, false, 0, NULL, NULL, NULL, false, 0, false, 0}
#define PlayerControl_init_zero                  {_PlayerControl_Command_MIN, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define PlayerControl_command_tag                1
#define PlayerControl_volume_tag                 2
#define PlayerInfo_state_tag                     1
#define PlayerInfo_volume_tag                    2
#define PlayerInfo_player_name_tag               3
#define PlayerInfo_song_title_tag                4
#define PlayerInfo_song_artist_tag               5
#define PlayerInfo_current_position_tag          6
#define PlayerInfo_duration_tag                  7
#define Media_player_info_tag                    1
#define Media_player_control_tag                 2

/* Struct field encoding specification for nanopb */
extern const pb_field_t Media_fields[3];
extern const pb_field_t PlayerInfo_fields[8];
extern const pb_field_t PlayerControl_fields[3];

/* Maximum encoded size of messages (where known) */
/* Media_size depends on runtime parameters */
/* PlayerInfo_size depends on runtime parameters */
#define PlayerControl_size                       8

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define WEAR_MEDIA_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
