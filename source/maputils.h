// Basically https://github.com/Atmosphere-NX/Atmosphere/blob/12236eae9eb02cb91cd1fc035281287ab7797c41/stratosphere/loader/source/ldr_map.cpp
#include <switch.h>

typedef struct
{
    u64 heap_base;
    u64 heap_size;
    u64 heap_end;
    u64 map_base;
    u64 map_size;
    u64 map_end;
    u64 addspace_base;
    u64 addspace_size;
    u64 addspace_end;
}AddressSpaceInfo;

Result GetAddressSpaceInfo(AddressSpaceInfo *out, Handle process_h);
Result LocateSpaceForMapDeprecated(u64 *out, u64 out_size);
Result LocateSpaceForMapModern(u64 *out, u64 out_size);
Result LocateSpaceForMap(u64 *out, u64 out_size);
