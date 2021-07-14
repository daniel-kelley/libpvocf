/*
   pvocf_internal.h

   Copyright (c) 2021 by Daniel Kelley

*/

#ifndef   PVOCF_INTERNAL_H_
#define   PVOCF_INTERNAL_H_

#include "pvocf.h"
#include "riffr.h"

#define PVOC_DATA_FORMAT "DDWWWWDDDDDD"

#define PVOC_SUBTYPE_GUID \
    { 0xc2, 0xb9, 0x12, 0x83, /*0x8312b9c2*/                    \
            0x6e, 0x2e, /*0x2e6e*/                              \
            0xd4, 0x11, /*0x11d4*/                              \
            0xa8, 0x24, 0xde, 0x5b, 0x96, 0xc3, 0xab, 0x21 }

struct pvocf {
    struct riffr *riffr;
    struct pvoc_info info;
};

#endif /* PVOCF_INTERNAL_H_ */
