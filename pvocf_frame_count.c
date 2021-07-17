/*
   pvocf_frame_count.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "pvocf.h"
#include "pvocf_internal.h"

int pvocf_frame_count(struct pvocf *handle)
{
    int rc = -1;

    if (handle) {
        rc = handle->info.data_size / handle->info.pvoc.dwFrameAlign;
    }

    return rc;
}
