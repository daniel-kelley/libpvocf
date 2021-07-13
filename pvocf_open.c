/*
   pvocf_open.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "pvocf_internal.h"

static uint8_t pvocf_guid[RIFFR_WAVE_FMT_GUID_LEN] = PVOC_SUBTYPE_GUID;

/* Attempt to read PVOCEX file header */
static int pvocf_open_pvocex(struct pvocf *handle, struct riffr_chunk_type *form)
{
    int err = -1;

    do {
        const char *form_str;
        const char *hdr_str;
        struct riffr_chunk_header header;
        struct riffr_chunk_type type;
        int fmt_actual;
        int ext_actual;
        ssize_t fmt_extension;

        form_str = riffr_type_str(*form);

        if (strcmp(form_str, "WAVE")) {
            /* not a WAVE file */
            break;
        }

        err = riffr_read_chunk_header(handle->riffr, &header);
        if (err) {
            break;
        }

        err = riffr_get_chunk_type(handle->riffr, header.id, &type);
        if (err) {
            break;
        }

        hdr_str = riffr_type_str(type);
        if (strcmp(hdr_str, "fmt ")) {
            /* no fmt chunk in WAVE file */
            break;
        }

        /* Read as much of fmt as possible up to the size of the header. */
        fmt_actual = riffr_read_data(handle->riffr,
                                     RIFFR_WAVE_FMT_FORMAT,
                                     header.size,
                                     &handle->fmt);

        if (fmt_actual < 0) {
            /* Error reading fmt chunk */
            err = -1;
            break;
        }

        if (fmt_actual != sizeof(handle->fmt)) {
            /* Error reading fmt chunk */
            err = -1;
            break;
        }

        if (handle->fmt.cbSize != 62) {
            /* FIXME: doube check */
            err = -1;
            break;
        }

        if (memcmp(handle->fmt.SubFormat, pvocf_guid, sizeof(pvocf_guid))) {
            err = -1;
            break;
        }

        fmt_extension = header.size - fmt_actual;

        ext_actual = riffr_read_data(handle->riffr,
                                     PVOC_DATA_FORMAT,
                                     fmt_extension,
                                     &handle->pvoc);
        if (ext_actual < 0) {
            err = -1;
            break;
        }

        if (ext_actual != fmt_extension) {
            err = -1;
            break;
        }

        err = 0; /*???*/
    } while (0);

    return err;
}

/* Attempt to open PVOCEX file 'filename' */
static int pvocf_open_internal(struct pvocf *handle, const char *filename)
{
    int rc = -1;
    struct riffr_chunk_type form;
    handle->riffr = riffr_open(filename, "r", &form);
    if (handle->riffr) {
        rc = pvocf_open_pvocex(handle, &form);
    } else {
        fprintf(stderr, "%s: %s\n",filename,strerror(errno));
    }

    return rc;
}

struct pvocf *pvocf_open(const char *filename)
{
    struct pvocf *handle;

    handle = calloc(1, sizeof(*handle));

    if (handle) {
        if (pvocf_open_internal(handle, filename)) {
            free(handle);
            handle = NULL;
        }
    }

    return handle;
}
