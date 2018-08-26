// Place this header in the same directory as your script for your 
// text-editor to pick up the autocompletion


typedef unsigned char u8; 
typedef unsigned short u16; 
typedef unsigned int u32; 
typedef unsigned long u64; 
typedef enum {
    MemType_Unmapped=0x00,
    MemType_Io=0x01,
    MemType_Normal=0x02,
    MemType_CodeStatic=0x03,
    MemType_CodeMutable=0x04,
    MemType_Heap=0x05,
    MemType_SharedMem=0x06,
    MemType_WeirdMappedMem=0x07,
    MemType_ModuleCodeStatic=0x08,
    MemType_ModuleCodeMutable=0x09,
    MemType_IpcBuffer0=0x0A,
    MemType_MappedMemory=0x0B,
    MemType_ThreadLocal=0x0C,
    MemType_TransferMemIsolated=0x0D,
    MemType_TransferMem=0x0E,
    MemType_ProcessMem=0x0F,
    MemType_Reserved=0x10,
    MemType_IpcBuffer1=0x11,
    MemType_IpcBuffer3=0x12,
    MemType_KernelStack=0x13,
    MemType_CodeReadOnly=0x14,
    MemType_CodeWritable=0x15
} MemoryType;


typedef struct {
    MemoryType memType;
    u64 address;
    u64 size;
} MemRegion;

// Sleeps for time milliseconds 
u32 sleepMS(int time);

// Gets the current system tick
// ((double) tick)/19200000 == time in seconds
u64 svcGetSystemTick();


// Reads game-memory
u64 readMem(void* buffer, u64 address, u64 size);

// Writes game-memory
u64 writeMem(void* buffer, u64 address, u64 size);


// Macros for easy reading+writing 
// Look at example for usage
#define peek(target, address) (readMem(target, address, sizeof(*target)))
#define poke(target, address) (writeMem(target, address, sizeof(*target)))


// Finds the in n_th region of type memType
MemRegion findRegion(MemoryType memType, int n);

// Reads a line from the cli and puts maxchars of it into the outstring
int recvLine(char* outstring, int maxchars);