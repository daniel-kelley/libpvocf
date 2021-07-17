/*
   pvocf-info.c

   Copyright (c) 2021 by Daniel Kelley

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "pvocf.h"

struct info {
    int verbose;
    int show_analysis;
    uint32_t frame_count;
    uint32_t frame_id;
    uint32_t frame_fmt;
    uint32_t bin_count;
};

#define BIN_SIZE (2*sizeof(float))

static int show_analysis(struct pvocf *handle, const struct info *opt)
{
    int err = 1;
    float *frame = NULL;
    int frame_count = pvocf_frame_count(handle);
    int frame_size = pvocf_frame_size(handle);
    int i;
    int j;
    const struct pvoc_info *info = pvocf_get_info(handle);
    int bin_count = info->pvoc.nAnalysisBins;
    float *f;

    do {
        if (frame_count < 0) {
            /* Error getting frame count. */
            break;
        }

        if (frame_size < 0) {
            /* Error getting frame size. */
            break;
        }

        if (opt->frame_count) {
            frame_count = opt->frame_count;
        }

        if (opt->bin_count) {
            bin_count = opt->bin_count;
        }

        frame = calloc(frame_count, frame_size);
        if (!frame) {
            break;
        }

        err = pvocf_get_frame(handle,
                              opt->frame_id,
                              frame_count,
                              frame_count * frame_size,
                              frame);
        if (err) {
            break;
        }

        printf("data:\n");

        for (f = frame, i=0; i<frame_count; i++) {
            printf("# frame %d [amp,freq]\n", opt->frame_id + i);
            printf("-\n");
            for (j=0; j<frame_size; j += (BIN_SIZE)) {
                float amp = *f++;
                float freq = *f++;
                if (j >= (bin_count * (int)BIN_SIZE)) {
                    continue;
                }
                if (opt->frame_fmt) {
                    printf("  - [%.3f, %.3f]\n", amp, freq);
                } else {
                    printf("  - [%g, %g]\n", amp, freq);
                }
            }
        }
        free(frame);
    } while (0);

    return err;
}

static int show_info(struct pvocf *handle)
{
    int err = 1;
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
        printf("data.size: %d\n",info->data_size);
        err = 0;
    }

    return err;
}

static int pvocf_info(const char *file, const struct info *opt)
{
    int err = 1;
    struct pvocf *handle = pvocf_open(file);
    if (handle) {
        err = show_info(handle);
        if (!err && opt->show_analysis) {
            err = show_analysis(handle, opt);
        }
    } else {
        fprintf(stderr, "%s: error\n", file);
    }
    pvocf_close(handle);

    return err;
}

static void usage(const char *prog)
{
    fprintf(stderr,"%s -f n [-vh] <input>\n", prog);
    fprintf(stderr,"  -h        Print this message\n");
    fprintf(stderr,"  -n n      Start with analysis frame n\n");
    fprintf(stderr,"  -a n      Show n analysis frames\n");
    fprintf(stderr,"  -b n      Show first n analysis bins\n");
    fprintf(stderr,"  -A        Show analysis frames\n");
    fprintf(stderr,"  -F        Use %%.3f for analysis float format\n");
    fprintf(stderr,"  -v        Verbose\n");
}

int main(int argc, char *argv[])
{
    int err = 1;
    int i;
    int c;
    struct info info;

    memset(&info, 0, sizeof(info));
    while ((c = getopt(argc, argv, "a:n:b:AFvh")) != EOF) {
        switch (c) {
        case 'a':
            info.frame_count = strtoul(optarg, NULL, 0);
            break;
        case 'b':
            info.bin_count = strtoul(optarg, NULL, 0);
            break;
        case 'A':
            info.show_analysis = 1;
            break;
        case 'n':
            info.frame_id = strtoul(optarg, NULL, 0);
            break;
        case 'v':
            info.verbose = 1;
            break;
        case 'F':
            info.frame_fmt = 1;
            break;
        case 'h':
            usage(argv[0]);
            err = EXIT_SUCCESS;
            break;
        default:
            break;
        }
    }

    for (i=optind; i<argc; i++) {
        err = pvocf_info(argv[i], &info);
        if (err) {
            break;
        }
    }

    return err;
}
