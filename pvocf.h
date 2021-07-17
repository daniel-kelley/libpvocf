/*
   pvocf.h

   Copyright (c) 2021 by Daniel Kelley

   Some data extracted from csound/pvfileio.h

*/

#ifndef   PVOCF_H_
#define   PVOCF_H_

#include <stdio.h>
#include <inttypes.h>
#include "riffr.h"

struct pvocf;

struct pvoc_data {   /* 40 bytes */
    uint32_t    dwVersion;
    uint32_t    dwDataSize;
    uint16_t    wWordFormat;    /* pvoc_wordformat                           */
    uint16_t    wAnalFormat;    /* pvoc_frametype                            */
    uint16_t    wSourceFormat;  /* WAVE_FORMAT_PCM or WAVE_FORMAT_IEEE_FLOAT */
    uint16_t    wWindowType;    /* pvoc_windowtype                           */
    uint32_t    nAnalysisBins;  /* implicit FFT size = (nAnalysisBins-1) * 2 */
    uint32_t    dwWinlen;       /* analysis winlen, in samples               */
                                /*   NB may be != FFT size                   */
    uint32_t    dwOverlap;      /* samples                                   */
    uint32_t    dwFrameAlign;   /* usually nAnalysisBins * 2 * sizeof(float) */
    float       fAnalysisRate;
    float       fWindowParam;   /* default 0.0f unless needed                */
};

struct pvoc_info {
    struct riffr_wave_fmt fmt;
    struct pvoc_data pvoc;
    uint32_t data_size;
};

extern struct pvocf *pvocf_open(const char *filename);
extern int pvocf_frame_count(struct pvocf *handle);
extern int pvocf_frame_size(struct pvocf *handle);
extern int pvocf_get_frame(struct pvocf *handle,
                           uint32_t frame_id,
                           uint32_t num_frame,
                           size_t size,
                           float *frame);
extern const struct pvoc_info *pvocf_get_info(struct pvocf *handle);

extern int pvocf_close(struct pvocf *handle);

#endif /* PVOCF_H_ */
