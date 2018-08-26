// This script sets the coins in Super Mario Oddysey Odyssey to the user-selected value

#include <stdio.h>
#include <stdlib.h>
#include <netcheat.h>

char line[100];

int main()
{
    MemRegion reg = findRegion(MemType_CodeMutable, 1);
    
    u64 ptrAddr = reg.address + 8589240;
    
    u64 ptrVal;
    peek(&ptrVal, ptrAddr);
    
    u64 coinAddr = ptrVal + 5240856;

    printf("Enter the new value\n");
    recvLine(line, 100);

    u32 newVal = strtoul(line, NULL, 10);
    poke(&newVal, coinAddr);

    return 0;
}