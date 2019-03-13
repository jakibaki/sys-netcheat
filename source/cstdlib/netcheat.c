#include "../interpreter.h"
#include "../picochelper.h"
#include "../util.h"
#include "../cheat.h"
#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef BUILTIN_MINI_STDLIB
#ifndef NO_FP

int scriptSock = 0;

void NetcheatSleepMS(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    svcSleepThread(1000000L * Param[0]->Val->Integer);
    ReturnValue->Val->UnsignedInteger = 0;
}

void NetcheatSvcGetSystemTick(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedLongInteger = svcGetSystemTick();
}

// Only listen on one port at once or make really sure that you're properly cleaning that up in the script!
void NetcheatStartListening(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = setupServerSocket(Param[0]->Val->Integer);
    scriptSock = ReturnValue->Val->Integer;
}

// int recv(int sockfd, void* buf, int len, int flags);
void NetcheatRecv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = recv(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer);
}

// int send(int sockfd, void* buf, int len, int flags);
void NetcheatSend(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = send(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer);
}

// int accept(int sockfd);
// Hint: If accept returns a number <= 0 that usually means that your switch went to sleep and you need to set up the socket again.
void NetcheatAccept(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    int c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    printf("Trying to accept on %d\n", Param[0]->Val->Integer);
    ReturnValue->Val->Integer = accept(Param[0]->Val->Integer, (struct sockaddr *)&client, (socklen_t *)&c);
    printf("Actually got connection :O\n");
}

// TODO: Close


// For now this uses the old debug-stuff sicne I can't figure out how to do it properly :/

// u64 rc = readMem(void* buffer, u64 address, u64 size)
void NetcheatReadMem(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mutexLock(&actionLock);
    attach();
    ReturnValue->Val->UnsignedLongInteger = svcReadDebugProcessMemory(Param[0]->Val->Pointer, debughandle, Param[1]->Val->UnsignedLongInteger, Param[2]->Val->UnsignedLongInteger);
    detach();
    mutexUnlock(&actionLock);
}

// u64 rc = writeMem(void* buffer, u64 address, u64 size);
void NetcheatWriteMem(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mutexLock(&actionLock);
    attach();
    ReturnValue->Val->UnsignedLongInteger = svcWriteDebugProcessMemory(debughandle, Param[0]->Val->Pointer, Param[1]->Val->UnsignedLongInteger, Param[2]->Val->UnsignedLongInteger);
    detach();
    mutexUnlock(&actionLock);
}

// MemRegion mr = findRegion(MemoryType memType, int index);
void NetcheatFindRegion(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mutexLock(&actionLock);
    attach();
    MemoryInfo meminf = getRegionOfType(Param[1]->Val->Integer, Param[0]->Val->UnsignedInteger);
    MemRegion memReg;
    memReg.memType = meminf.type;
    memReg.address = meminf.addr;
    memReg.size = meminf.size;
    ReturnValue->Val->MemoryRegion = memReg;
    detach();
    mutexUnlock(&actionLock);
}

void NetcheatRecvLine(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mutexLock(&actionLock);
    while (strlen(line) == 0)
    {
        mutexUnlock(&actionLock);
        svcSleepThread(200000000);
        mutexLock(&actionLock);

        if (semaphoreTryWait(&done))
        {
            semaphoreSignal(&done);
            break;
        }
    }
    mutexUnlock(&actionLock);
    char* outline = Param[0]->Val->Pointer;
    int maxLen = Param[1]->Val->Integer;
    strncpy(outline, line, maxLen);
    line[0] = 0;
    ReturnValue->Val->Integer = strlen(outline);
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
    u64 address;\
    u64 size;\
} MemRegion;\
typedef	unsigned int	nfds_t;\
struct pollfd {\
	int	fd;\
	short	events;\
	short	revents;\
};\
#define peek(target, address) (readMem(target, address, sizeof(*target)))\n\
#define poke(target, address) (writeMem(target, address, sizeof(*target)))\
";

struct LibraryFunction NetcheatFunctions[] =
    {
        {NetcheatSleepMS, "u32 sleepMS(int);"},
        {NetcheatSvcGetSystemTick, "u64 svcGetSystemTick();"},
        {NetcheatStartListening, "int startListening(int);"},
        {NetcheatRecv, "int recv(int, void*, int, int);"},
        {NetcheatSend, "int send(int, void*, int, int);"},
        {NetcheatAccept, "int accept(int);"},
        {NetcheatReadMem, "u64 readMem(void*, u64, u64);"},
        {NetcheatWriteMem, "u64 writeMem(void*, u64, u64);"},
        {NetcheatFindRegion, "MemRegion findRegion(MemoryType, int);"},
        {NetcheatRecvLine, "int recvLine(char*, int);"},
        {NULL, NULL}};

static int POLLINValue = POLLIN;         /* any readable data available */
static int POLLPRIValue = POLLPRI;       /* OOB/Urgent readable data */
static int POLLOUTValue = POLLOUT;       /* file descriptor is writeable */
static int POLLRDNORMValue = POLLRDNORM; /* non-OOB/URG data available */
static int POLLWRNORMValue = POLLWRNORM; /* no write type differentiation */
static int POLLRDBANDValue = POLLRDBAND; /* OOB/Urgent readable data */
static int POLLWRBANDValue = POLLWRBAND;

void NetcheatSetupFunc(Picoc *pc)
{
    VariableDefinePlatformVar(pc, NULL, "POLLIN", &pc->IntType, (union AnyValue *)&POLLINValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLPRI", &pc->IntType, (union AnyValue *)&POLLPRIValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLOUT", &pc->IntType, (union AnyValue *)&POLLOUTValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLRDNORM", &pc->IntType, (union AnyValue *)&POLLRDNORMValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLWRNORM", &pc->IntType, (union AnyValue *)&POLLWRNORMValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLRDBAND", &pc->IntType, (union AnyValue *)&POLLRDBANDValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "POLLWRBAND", &pc->IntType, (union AnyValue *)&POLLWRBANDValue, FALSE);
}

#endif /* !NO_FP */
#endif /* !BUILTIN_MINI_STDLIB */
