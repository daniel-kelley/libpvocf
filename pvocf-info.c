/*
   pvocf-info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "pvocf.h"

static void show_info(struct pvocf *handle)
{
    const struct pvoc_info *info = pvocf_get_info(handle);
    if (info) {
        printf("fmt.nChannels: %d\n",info->fmt.nChannels);
        printf("fmt.nSamplesPerSec: %d\n",info->fmt.nSamplesPerSec);
        printf("fmt.wBitsPerSample: %d\n",info->fmt.wBitsPerSample);
        printf("fmt.wValidBitsPerSample: %d\n",info->fmt.wValidBitsPerSample);
        printf("fmt.dwChannelMask: 0x%08x\n",info->fmt.dwChannelMask);
        printf("pvoc.dwVersion: %d\n",info->pvoc.dwVersion);
        printf("pvoc.dwDataSize: %d\n",info->pvoc.dwDataSize);
        printf("pvoc.wWordFormat: %d\n",info->pvoc.wWordFormat);
        printf("pvoc.wAnalFormat: %d\n",info->pvoc.wAnalFormat);
        printf("pvoc.wSourceFormat: %d\n",info->pvoc.wSourceFormat);
        printf("pvoc.wWindowType: %d\n",info->pvoc.wWindowType);
        printf("pvoc.nAnalysisBins: %d\n",info->pvoc.nAnalysisBins);
        printf("pvoc.dwWinlen: %d\n",info->pvoc.dwWinlen);
        printf("pvoc.dwOverlap: %d\n",info->pvoc.dwOverlap);
        printf("pvoc.dwFrameAlign: %d\n",info->pvoc.dwFrameAlign);
        printf("pvoc.fAnalysisRate: %g\n",info->pvoc.fAnalysisRate);
        printf("pvoc.fWindowParam: %g\n",info->pvoc.fWindowParam);
    }
}

static void pvocf_info(const char *file)
{
    struct pvocf *handle = pvocf_open(file);
    if (handle) {
        show_info(handle);
    } else {
        fprintf(stderr, "%s: error\n", file);
    }
    pvocf_close(handle);
}

int main(int argc, char *argv[])
{
    int i;

    for (i=1; i<argc; i++) {
        pvocf_info(argv[i]);
    }

    return 0;
}
