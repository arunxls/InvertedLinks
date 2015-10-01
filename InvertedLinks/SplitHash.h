#pragma once
#include "ArrayGraphReader.h"
#include "ArrayHashCountReader.h"
#include <vector>
#include <string>

#define READBUFFER 32

class SplitHash
{
public:
    char* filename;
    void* buffer_start;
    void* buffer_end;
    std::vector<std::string> merge_files;

    ArrayGraphReader* graph;
    ArrayHashCountReader* hashCount;

    SplitHash(char*, void*, void*);
    ~SplitHash();
    void execute();

};

