//------------------------------------------------------------------------------
// File: vputypes.h
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef _JPU_TYPES_H_
#define _JPU_TYPES_H_


typedef unsigned char   uint8_t;
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
// typedef char            int8_t;
typedef int             int32_t;
typedef short           int16_t;


typedef unsigned long long uint64_t;
typedef long long int64_t;

#ifndef PhysicalAddress
typedef uint32_t PhysicalAddress;
#endif
#ifndef BYTE
typedef unsigned char   BYTE;
#endif
#ifndef BOOL
typedef int BOOL;
#endif

#ifndef FALSE
#define FALSE           0
#endif /* FALSE */
#ifndef TRUE
#define TRUE            1
#endif /* TRUE */
#ifndef NULL
#define NULL    0
#endif


#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) { (P) = (P); }
#endif

#endif  /* _JPU_TYPES_H_ */
