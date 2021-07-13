/*
   pvocf_get_info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "pvocf_internal.h"
#include "pvocf.h"

const struct pvoc_data *pvocf_get_info(struct pvocf *handle)
{
    struct pvoc_data *data = NULL;
    if (handle) {
        data = &handle->pvoc;
    }
    return data;
}
