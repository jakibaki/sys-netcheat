#pragma once
#include <switch.h>

extern int sock;
extern int scriptSock;
extern Semaphore done;

int picocRunPath(char* path);

typedef struct
{
    MemoryType memType;
    u64 address;
    u64 size;
} MemRegion;
