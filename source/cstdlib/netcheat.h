#include <switch.h>

typedef struct
{
    MemoryType memType;
    void* address;
    u64 size;
} MemRegion;

Semaphore testSem;