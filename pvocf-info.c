/*
   pvocf-info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include "pvocf.h"

static void show_info(struct pvocf *handle)
{
    const struct pvoc_data *data = pvocf_get_info(handle);
    if (data) {
        printf("dwVersion: %d\n",data->dwVersion);
        printf("dwDataSize: %d\n",data->dwDataSize);
        printf("wWordFormat: %d\n",data->wWordFormat);
        printf("wAnalFormat: %d\n",data->wAnalFormat);
        printf("wSourceFormat: %d\n",data->wSourceFormat);
        printf("wWindowType: %d\n",data->wWindowType);
        printf("nAnalysisBins: %d\n",data->nAnalysisBins);
        printf("dwWinlen: %d\n",data->dwWinlen);
        printf("dwOverlap: %d\n",data->dwOverlap);
        printf("dwFrameAlign: %d\n",data->dwFrameAlign);
        printf("fAnalysisRate: %g\n",data->fAnalysisRate);
        printf("fWindowParam: %g\n",data->fWindowParam);
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
