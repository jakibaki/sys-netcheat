#include "../interpreter.h"
#include <switch.h>
#include <stdio.h>

#ifndef BUILTIN_MINI_STDLIB
#ifndef NO_FP

void NetcheatSleepMS(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedInteger = svcSleepThread(1000000L * Param[0]->Val->Integer);
}

void NetcheatSvcGetSystemTick(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedLongInteger = svcGetSystemTick();
}


const char NetcheatDefs[] = "\
typedef unsigned char u8; \
typedef unsigned short u16; \
typedef unsigned int u32; \
typedef unsigned long u64; \
typedef enum {\
    MemType_Unmapped=0x00,\
    MemType_Io=0x01,\
    MemType_Normal=0x02,\
    MemType_CodeStatic=0x03,\
    MemType_CodeMutable=0x04,\
    MemType_Heap=0x05,\
    MemType_SharedMem=0x06,\
    MemType_WeirdMappedMem=0x07,\
    MemType_ModuleCodeStatic=0x08,\
    MemType_ModuleCodeMutable=0x09,\
    MemType_IpcBuffer0=0x0A,\
    MemType_MappedMemory=0x0B,\
    MemType_ThreadLocal=0x0C,\
    MemType_TransferMemIsolated=0x0D,\
    MemType_TransferMem=0x0E,\
    MemType_ProcessMem=0x0F,\
    MemType_Reserved=0x10,\
    MemType_IpcBuffer1=0x11,\
    MemType_IpcBuffer3=0x12,\
    MemType_KernelStack=0x13,\
    MemType_CodeReadOnly=0x14,\
    MemType_CodeWritable=0x15\
} MemoryType;\
typedef struct {\
    MemoryType memType;\
    void* address;\
    u64 size;\
} MemRegion;\
";

struct LibraryFunction NetcheatFunctions[] =
{
    {NetcheatSleepMS, "u32 sleepMS(int);"},
    {NetcheatSvcGetSystemTick, "u64 svcGetSystemTick();"},
    {NULL, NULL}
};

void NetcheatSetupFunc(Picoc *pc)
{
}

#endif /* !NO_FP */
#endif /* !BUILTIN_MINI_STDLIB */
