/*
   pvocf.h

   Copyright (c) 2021 by Daniel Kelley

   Some data extracted from csound/pvfileio.h

*/

#ifndef   PVOCF_H_
#define   PVOCF_H_

#include <stdio.h>
#include <inttypes.h>

struct pvocf;

typedef struct
{
    uint32_t        Data1;
    uint16_t        Data2;
    uint16_t        Data3;
    unsigned char   Data4[8];
} GUID;

typedef struct /* waveformatex */ {
    uint16_t    wFormatTag;
    uint16_t    nChannels;
    uint32_t    nSamplesPerSec;
    uint32_t    nAvgBytesPerSec;
    uint16_t    nBlockAlign;
    uint16_t    wBitsPerSample;
    uint16_t    cbSize;
} WAVEFORMATEX;

typedef struct pvoc_data {   /* 32 bytes */
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
} PVOCDATA;

#define PVOC_SUBTYPE_GUID \
    0x8312b9c2, 0x2e6e, 0x11d4, \
    { 0xa8, 0x24, 0xde, 0x5b, 0x96, 0xc3, 0xab, 0x21 }

typedef struct pvoc_file {
    WAVEFORMATEX fmtdata;
    PVOCDATA    pvdata;
    int32_t       datachunkoffset;
    int32_t       nFrames;        /* no of frames in file */
    int32_t       FramePos;       /* where we are in file */
    FILE        *fp;
    void        *fd;
    int32_t       curpos;
    int32_t         to_delete;
    int32_t         readonly;
    char        *name;
    float       *customWindow;
} PVOCFILE;

extern int pvocf_open(struct pvocf *handle, const char *filename);
extern int pvocf_frame_count(struct pvocf *handle);
extern int pvocf_get_frame(struct pvocf *handle,
                           uint32_t frame_id,
                           uint32_t num_frame,
                           float *frame);
extern int pvocf_close(struct pvocf *handle);

#endif /* PVOCF_H_ */
