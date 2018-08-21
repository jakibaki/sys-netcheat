#include <switch.h>
extern int sock;
extern Semaphore done;

int picocRunPath(char* path);

typedef struct
{
    MemoryType memType;
    void* address;
    u64 size;
} MemRegion;
