/*
   pvocf_get_info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "pvocf_internal.h"
#include "pvocf.h"

const struct pvoc_info *pvocf_get_info(struct pvocf *handle)
{
    struct pvoc_info *info = NULL;
    if (handle) {
        info = &handle->info;
    }
    return info;
}
