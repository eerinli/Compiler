#include <iostream>
#include <cstdint>
#include "a10p1.h"

// The program implement functions, in terms of a10p1.o, using the free list algorithm:
int64_t* curAddress = arena();

int64_t* mymalloc(int64_t size) {
    int64_t byteSize = size * 8;
    int64_t byteSizeMore = byteSize + 8;
    int64_t* curBlock = curAddress;
    int64_t* address;
    if (*curBlock != byteSize && *curBlock != byteSizeMore) {
        int64_t prevBlock;
        if (byteSize >= *curBlock) {
            int64_t* nextBlock = (int64_t*)*(curBlock + 1);
            while (*nextBlock < byteSize) {
                if ((int64_t*)*(nextBlock + 1) == 0) return 0;
                curBlock = nextBlock;
                nextBlock = (int64_t*)*(nextBlock + 1);
            }
            prevBlock = *nextBlock;
            *nextBlock = byteSize;
            address = 1 + nextBlock;
            if (prevBlock != byteSize && prevBlock != byteSizeMore) {
                *(address + 1 + size) = *address;
                *(address + size) = prevBlock - (1 + size) * 8;
                *(curBlock + 1) = (int64_t)(address + size);
            } else {
                *(curBlock + 1) = *address;
                nextBlock = (int64_t*)*address;
            }
        } else {
            prevBlock = *curBlock;
            *curBlock = byteSize;
            address = curBlock + 1;
            curAddress = address + size;
            *(curAddress + 1) = *(curBlock + 1);
            *curAddress = prevBlock - (1 + size) * 8;
        }
    } else {
        address = curBlock + 1;
        curAddress = (int64_t*)*(curBlock + 1);
    }
    return address;
}

void myfree(int64_t* address) {
    if (address != nullptr) {  // ensure it is not NULL
        int64_t* prevAddress = curAddress;
        curAddress = address - 1;
        *(curAddress + 1) = (int64_t)prevAddress;
    }
}
