// This script sets the coins in Super Mario Oddysey Odyssey to 5000

#include <stdio.h>
#include <netcheat.h>

int main()
{
    MemRegion reg = findRegion(MemType_CodeMutable, 1);
    printf("%p\n", (void*)reg.address);
    u64 ptrAddr = reg.address + 8589240;
    u64 ptrVal;
    readMem(&ptrVal, ptrAddr, sizeof(u64));
    u64 coinAddr = ptrVal + 5240856;

    u32 newVal = 5000;
    writeMem(&newVal, coinAddr, sizeof(u32));

    return 0;
}