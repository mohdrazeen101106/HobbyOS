#include "kernel/lib/kstring.h"

size_t kstrlcpy(char *dst, const char *src, size_t size) {
    size_t srclen = 0;
    while (src[srclen]) ++srclen;

    if (size > 0) {
        size_t copylen = (srclen < size - 1) ? srclen : size - 1;
        for (size_t i = 0; i < copylen; ++i) dst[i] = src[i];
        dst[copylen] = '\0';
    }
    return srclen;
}