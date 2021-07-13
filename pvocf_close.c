/*
   pvocf_close.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include "riffr.h"
#include "pvocf_internal.h"

int pvocf_close(struct pvocf *handle)
{
    int rc = 0;

    if (handle) {
        rc = riffr_close(handle->riffr);
        memset(handle, 0, sizeof(*handle));
        free(handle);
    }

    return rc ? 1 : 0;
}
