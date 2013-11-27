//  pcq.h
//  Two-thread-safe, lock-free producer-consumer ring buffer queue

#ifndef __pcring_h__
#define __pcring_h__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef int64_t pcqsize_t;

typedef struct {
    uintmax_t w;
    uintmax_t r;
    pcqsize_t nbytes;
    pcqsize_t size;
    uint8_t *buf;
} pcq_t;

#ifdef __cplusplus
extern "C" {
#endif
    
    pcq_t *pcq_alloc(size_t size);
    void pcq_free(pcq_t *pcq);    
    
    int test_pcq();

    static inline void *pcq_writeloc(pcq_t *_this) {
        return &_this->buf[_this->w];
    }
    
    static inline void *pcq_readloc(pcq_t *_this) {
        return &_this->buf[_this->r];
    }
    
    static inline pcqsize_t pcq_bytes_remaining(pcq_t *_this) {
        return _this->size - _this->nbytes;
    }
    
    static inline void pcq_advance_write(pcq_t *_this, size_t nbytes) {
        assert(nbytes >= 0 && nbytes <= pcq_bytes_remaining(_this));
        if (nbytes == 0) return;
        
        __sync_add_and_fetch(&_this->nbytes, nbytes);
        
        _this->w += nbytes;
        if (_this->w > _this->size) _this->w -= _this->size;
    }
    
    static inline void pcq_advance_read(pcq_t *_this, size_t nbytes) {
        assert(nbytes >= 0 && nbytes <= _this->nbytes);
        if (nbytes == 0) return;

        __sync_sub_and_fetch(&_this->nbytes, nbytes);

        _this->r += nbytes;
        if (_this->r > _this->size) _this->r -= _this->size;
    }
    

#ifdef __cplusplus
}
#endif

#endif
