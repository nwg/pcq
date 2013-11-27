
#ifndef __mach_mirrored_alloc_h__
#define __mach_mirrored_alloc_h__

#include <mach/mach.h>

void *mach_mirrored_alloc(vm_offset_t nbytes);
void mach_mirrored_dealloc(void *address, vm_offset_t nbytes);

#endif
