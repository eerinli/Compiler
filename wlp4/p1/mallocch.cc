#include <cstdint>
#include <iostream>

#include "a10p1.h"

int64_t* currAddress = arena();

int64_t* mymalloc(int64_t size) {
    int64_t space = size * 8;
    int64_t* currBlock = currAddress;
    int64_t* address;
    if (*currBlock == space || *currBlock == space + 8) {
        address = currBlock + 1;
        currAddress = (int64_t*)*(currBlock + 1);
    } else {
        int64_t prevBlock;
        if (space < *currBlock) {
            prevBlock = *currBlock;
            *currBlock = space;
            address = currBlock + 1;
            currAddress = address + size;
            *currAddress = prevBlock - (size + 1) * 8;
            *(currAddress + 1) = *(currBlock + 1);
        } else {
            int64_t* nextBlock = (int64_t*)*(currBlock + 1);
            while (space > *nextBlock) {
                currBlock = nextBlock;
                nextBlock = (int64_t*)*(nextBlock + 1);
                if (nextBlock == 0) return 0;
            }
            prevBlock = *nextBlock;
            *nextBlock = space;
            address = nextBlock + 1;
            if (prevBlock == space || prevBlock == space + 8) {
                *(currBlock + 1) = *address;
                nextBlock = (int64_t*)*address;
            } else {
                *(address + size) = prevBlock - (size + 1) * 8;
                *(address + size + 1) = *address;
                *(currBlock + 1) = (int64_t)(address + size);
            }
        }
    }
    return address;
}

void myfree(int64_t* address) {
    if (address) {  // ensure it is not NULL
        int64_t* prevAddress = currAddress;
        currAddress = address - 1;
        *(currAddress + 1) = (int64_t)prevAddress;
    }
}
