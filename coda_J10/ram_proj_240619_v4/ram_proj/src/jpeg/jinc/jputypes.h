//------------------------------------------------------------------------------
// File: vputypes.h
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef _JPU_TYPES_H_
#define _JPU_TYPES_H_


typedef unsigned char   Uint8;
typedef unsigned int    Uint32;
typedef unsigned short  Uint16;
typedef char            Int8;
typedef int             Int32;
typedef short           Int16;
#if defined(_MSC_VER)
typedef unsigned _int64 Uint64;
typedef _int64 Int64;
#else
typedef unsigned long long Uint64;
typedef long long Int64;
#endif
#ifndef PhysicalAddress
typedef Uint32 PhysicalAddress;
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
