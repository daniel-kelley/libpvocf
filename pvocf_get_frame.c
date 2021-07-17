/*
   pvocf_get_frame.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include "pvocf.h"
#include "riffr.h"
#include "pvocf_internal.h"

static int pvocf_skip_frames(struct pvocf *handle, uint32_t num_frame)
{
    int err = 0;
    size_t len = pvocf_frame_size(handle) * num_frame;
    while (num_frame) {
        err = riffr_read_chunk_body(handle->riffr, len, NULL);
        if (err) {
            break;
        }
        num_frame--;
    }

    return err;
}

static int pvocf_get_frame_internal(struct pvocf *handle,
                                size_t len,
                                float *buffer)
{
    return riffr_read_chunk_body(handle->riffr, len, buffer);
}

int pvocf_get_frame(struct pvocf *handle,
                    uint32_t frame_id,
                    uint32_t num_frame,
                    size_t size,
                    float *frame)
{
    int err = -1;
    int frames = 0;
    size_t frame_size = 0;

    do {
        if (!handle) {
            /* handle is NULL */
            break;
        }

        if (!frame) {
            /* frame output pointer is NULL */
            break;
        }

        frame_size = pvocf_frame_size(handle);

        frames = pvocf_frame_count(handle);
        if (frames < 0) {
            /* error getting frame count */
            break;
        }

        if ((frame_id + num_frame) > (uint32_t)frames) {
            /* frame range out of bounds */
            break;
        }

        if (size < frame_size * num_frame) {
            /* frame output buffer too small */
            break;
        }

        if (handle->info.pvoc.wWordFormat != 0) {
            /* unsupported word format */
            break;
        }

        if (handle->info.pvoc.wAnalFormat != 0) {
            /* unsupported analysis format */
            break;
        }

        err = pvocf_skip_frames(handle, frame_id);
        if (err) {
            break;
        }

        err = pvocf_get_frame_internal(handle, num_frame * frame_size, frame);

    } while (0);

    return err;
}
