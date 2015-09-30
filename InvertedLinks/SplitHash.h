#pragma once
#include "ArrayGraphReader.h"
#include "ArrayHashCountReader.h"

#define READBUFFER 32

class SplitHash
{
public:
    char* filename;
    void* buffer_start;
    void* buffer_end;

    ArrayGraphReader* graph;
    ArrayHashCountReader* hashCount;

    SplitHash(char*, void*, void*);
    ~SplitHash();
    void execute();
};

