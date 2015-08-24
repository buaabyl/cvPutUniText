#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iconv.h"

char* libcodec_iconv(
        const char* instr,
        int inlen,
        int* outlen,
        const char* inenc,
        const char* outenc)
{
    char* inbuf;
    char* outbuf;
    size_t bytes_in;
    size_t bytes_left;

    char* pbuf;
    int nbuf;

    iconv_t ctl;

    ctl = iconv_open(outenc, inenc);
    if (ctl <= 0) {
        return NULL;
    }

    //prepare out buffer
    nbuf = inlen * 8;
    pbuf = malloc(nbuf + 2);
    memset(pbuf, 0, nbuf + 2);

    //convert, n_mbcs means bytes left.
    inbuf  = (char*)instr;
    outbuf = pbuf;
    bytes_in   = inlen;
    bytes_left = nbuf;

    if (iconv(ctl, &inbuf, &bytes_in, &outbuf, &bytes_left) != 0) {
        return 0;
    }
    *outlen = nbuf - bytes_left;

    iconv_close(ctl);
    ctl = 0;

    return pbuf;
}

void libcodec_free(char* s)
{
    if (s != NULL) {
        free(s);
    }
}

