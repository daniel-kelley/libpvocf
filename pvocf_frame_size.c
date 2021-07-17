/*
   pvocf_frame_size.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "pvocf.h"
#include "pvocf_internal.h"

int pvocf_frame_size(struct pvocf *handle)
{
    int rc = -1;

    if (handle) {
        rc = handle->info.pvoc.dwFrameAlign;
    }

    return rc;
}
