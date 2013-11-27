//
//  pcq.c
//

#include "pcq.h"
#include <assert.h>
#include <unistd.h>

#if __MACH__
#include <libkern/OSAtomic.h>
#include "mach_mirrored_alloc.h"
#define MIRRORED_ALLOC mach_mirrored_alloc
#define MIRRORED_DEALLOC mach_mirrored_dealloc
#else
#error platform def required
#endif

pcq_t *pcq_alloc(size_t size) {
    pcq_t *this = malloc(sizeof(pcq_t));
    memset(this, 0, sizeof(pcq_t));
    this->size = size;
    this->buf = MIRRORED_ALLOC(this->size);
    this->w = 0;
    this->r = 0;
    this->nbytes = 0;
    
    return this;
}

void pcq_free(pcq_t *this) {
    MIRRORED_DEALLOC(this->buf, this->size);
    free(this);
}
