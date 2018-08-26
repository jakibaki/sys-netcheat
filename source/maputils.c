#include <switch.h>
#include "maputils.h"

Result GetAddressSpaceInfo(AddressSpaceInfo *out, Handle process_h) {
    Result rc;
    if (R_FAILED((rc = svcGetInfo(&out->heap_base, 4, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    if (R_FAILED((rc = svcGetInfo(&out->heap_size, 5, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    if (R_FAILED((rc = svcGetInfo(&out->map_base, 2, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    if (R_FAILED((rc = svcGetInfo(&out->map_size, 3, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    if (R_FAILED((rc = svcGetInfo(&out->addspace_base, 12, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    if (R_FAILED((rc = svcGetInfo(&out->addspace_size, 13, CUR_PROCESS_HANDLE, 0)))) {
        return rc;
    }
    out->heap_end = out->heap_base + out->heap_size;
    out->map_end = out->map_base + out->map_size;
    out->addspace_end = out->addspace_base + out->addspace_size;
    return 0;
}

Result LocateSpaceForMapDeprecated(u64 *out, u64 out_size) {
    MemoryInfo mem_info = {0};
    u32 page_info = 0;
    Result rc;
    
    u64 cur_base = 0x8000000ULL;
    if (R_FAILED((rc = svcQueryMemory(&mem_info, &page_info, cur_base)))) {
        return rc;
    }
    
    rc = 0xD001;
    while (true) {
        if (mem_info.type == 0x10) {
                return rc;
        }
        if (mem_info.type == 0 && mem_info.addr - cur_base + mem_info.size >= out_size) {
            *out = cur_base;
            return 0x0;
        }
        u64 mem_end = mem_info.addr + mem_info.size;
        if (mem_end < cur_base) {
            return rc;
        }
        if (mem_end >> 31) {
            break;
        }
        cur_base = mem_end;
        if (R_FAILED((rc = svcQueryMemory(&mem_info, &page_info, cur_base)))) {
            return rc;
        }
    }
    return rc;
}

Result LocateSpaceForMapModern(u64 *out, u64 out_size) {
    MemoryInfo mem_info = {0};
    AddressSpaceInfo address_space = {0};
    u32 page_info = 0;
    u64 cur_base = 0, cur_end = 0;
    Result rc;
    
    if (R_FAILED((rc = GetAddressSpaceInfo(&address_space, CUR_PROCESS_HANDLE)))) {
        return rc;
    }
    
    cur_base = address_space.addspace_base;
    
    rc = 0xD001;
    cur_end = cur_base + out_size;
    if (cur_end <= cur_base) {
        return rc;
    }
    
    while (true) {
        if (address_space.heap_size && (address_space.heap_base <= cur_end - 1 && cur_base <= address_space.heap_end - 1)) {
            /* If we overlap the heap region, go to the end of the heap region. */
            if (cur_base == address_space.heap_end) {
                return rc;
            }
            cur_base = address_space.heap_end;
        } else if (address_space.map_size && (address_space.map_base <= cur_end - 1 && cur_base <= address_space.map_end - 1)) {
            /* If we overlap the map region, go to the end of the map region. */
            if (cur_base == address_space.map_end) {
                return rc;
            }
            cur_base = address_space.map_end;
        } else {
            if (R_FAILED(svcQueryMemory(&mem_info, &page_info, cur_base))) {
                /* TODO: panic. */
            }
            if (mem_info.type == 0 && mem_info.addr - cur_base + mem_info.size >= out_size) {
                *out = cur_base;
                return 0x0;
            }
            if (mem_info.addr + mem_info.size <= cur_base) {
                return rc;
            }
            cur_base = mem_info.addr + mem_info.size;
            if (cur_base >= address_space.addspace_end) {
                return rc;
            }
        }
        cur_end = cur_base + out_size;
        if (cur_base + out_size <= cur_base) {
            return rc;
        }
    }   
}


