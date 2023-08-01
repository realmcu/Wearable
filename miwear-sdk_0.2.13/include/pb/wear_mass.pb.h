/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.6 at Fri Jun 10 15:10:20 2022. */

#ifndef PB_WEAR_MASS_PB_H_INCLUDED
#define PB_WEAR_MASS_PB_H_INCLUDED
#include <pb.h>

#include "wear_common.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _Mass_MassID
{
    Mass_MassID_PREPARE = 0,
    Mass_MassID_CONTROL = 1
} Mass_MassID;
#define _Mass_MassID_MIN Mass_MassID_PREPARE
#define _Mass_MassID_MAX Mass_MassID_CONTROL
#define _Mass_MassID_ARRAYSIZE ((Mass_MassID)(Mass_MassID_CONTROL+1))

typedef enum _MassControl_Op
{
    MassControl_Op_PAUSE = 1,
    MassControl_Op_CANCEL = 2
} MassControl_Op;
#define _MassControl_Op_MIN MassControl_Op_PAUSE
#define _MassControl_Op_MAX MassControl_Op_CANCEL
#define _MassControl_Op_ARRAYSIZE ((MassControl_Op)(MassControl_Op_CANCEL+1))

/* Struct definitions */
typedef struct _MassControl
{
    MassControl_Op op;
    uint8_t data_type;
    pb_bytes_array_t *data_id;
    /* @@protoc_insertion_point(struct:MassControl) */
} MassControl;

typedef struct _PrepareRequest
{
    uint8_t data_type;
    pb_bytes_array_t *data_id;
    uint32_t data_length;
    bool has_support_compress_mode;
    uint8_t support_compress_mode;
    /* @@protoc_insertion_point(struct:PrepareRequest) */
} PrepareRequest;

typedef struct _PrepareResponse
{
    pb_bytes_array_t *data_id;
    PrepareStatus prepare_status;
    bool has_select_compress_mode;
    uint8_t select_compress_mode;
    bool has_remained_data_length;
    uint32_t remained_data_length;
    bool has_expected_slice_length;
    uint32_t expected_slice_length;
    /* @@protoc_insertion_point(struct:PrepareResponse) */
} PrepareResponse;

typedef struct _Mass
{
    pb_size_t which_payload;
    union
    {
        PrepareRequest prepare_request;
        PrepareResponse prepare_response;
        MassControl mass_control;
    } payload;
    /* @@protoc_insertion_point(struct:Mass) */
} Mass;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Mass_init_default                        {0, {PrepareRequest_init_default}}
#define PrepareRequest_init_default              {0, NULL, 0, false, 0}
#define PrepareResponse_init_default             {NULL, _PrepareStatus_MIN, false, 0, false, 0, false, 0}
#define MassControl_init_default                 {_MassControl_Op_MIN, 0, NULL}
#define Mass_init_zero                           {0, {PrepareRequest_init_zero}}
#define PrepareRequest_init_zero                 {0, NULL, 0, false, 0}
#define PrepareResponse_init_zero                {NULL, _PrepareStatus_MIN, false, 0, false, 0, false, 0}
#define MassControl_init_zero                    {_MassControl_Op_MIN, 0, NULL}

/* Field tags (for use in manual encoding/decoding) */
#define MassControl_op_tag                       1
#define MassControl_data_type_tag                2
#define MassControl_data_id_tag                  3
#define PrepareRequest_data_type_tag             1
#define PrepareRequest_data_id_tag               2
#define PrepareRequest_data_length_tag           3
#define PrepareRequest_support_compress_mode_tag 4
#define PrepareResponse_data_id_tag              1
#define PrepareResponse_prepare_status_tag       2
#define PrepareResponse_select_compress_mode_tag 3
#define PrepareResponse_remained_data_length_tag 4
#define PrepareResponse_expected_slice_length_tag 5
#define Mass_prepare_request_tag                 1
#define Mass_prepare_response_tag                2
#define Mass_mass_control_tag                    3

/* Struct field encoding specification for nanopb */
extern const pb_field_t Mass_fields[4];
extern const pb_field_t PrepareRequest_fields[5];
extern const pb_field_t PrepareResponse_fields[6];
extern const pb_field_t MassControl_fields[4];

/* Maximum encoded size of messages (where known) */
/* Mass_size depends on runtime parameters */
/* PrepareRequest_size depends on runtime parameters */
/* PrepareResponse_size depends on runtime parameters */
/* MassControl_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define WEAR_MASS_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
