
#include "mach_mirrored_alloc.h"

#define NRETRIES 10

void *_mach_mirrored_alloc(vm_offset_t nbytes) {
    vm_address_t address;
    if (vm_allocate(mach_task_self(), &address, nbytes*2, TRUE) != KERN_SUCCESS) {
        return NULL;
    }
    
    vm_deallocate(mach_task_self(), address + nbytes, nbytes);
    
    vm_address_t mirror = address + nbytes;
    vm_prot_t curProtection, maxProtection;
    
    if (vm_remap(mach_task_self(), &mirror, nbytes, 0, FALSE, mach_task_self(), address, FALSE, &curProtection, &maxProtection, VM_INHERIT_SHARE) != KERN_SUCCESS) {
        vm_deallocate(mach_task_self(), address, nbytes);
        return NULL;
    }
    
    return (void*)address;
}

void *mach_mirrored_alloc(vm_offset_t nbytes) {
    void *result;
    for (int i = 0; i < NRETRIES; i++) {
        if ((result = _mach_mirrored_alloc(nbytes))) return result;
    }
    
    return NULL;
}

void mach_mirrored_dealloc(void *address, vm_offset_t nbytes) {
    vm_deallocate(mach_task_self(), (vm_address_t)address, nbytes);
}

