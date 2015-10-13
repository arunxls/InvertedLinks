#pragma once
#include "ArrayGraphReader.h"
#include "ArrayHashCountReader.h"
#include "ArrayHashCountManager.h"
#include <vector>
#include <string>

#define READBUFFER 32

class SplitHash
{
public:
    char* buffer_start;
    char* buffer_end;

    ArrayGraphReader* graph;
    FileReader *FR;
    ArrayHashCountManager* hashCountManager;

    uint64 total_read;
    uint64 total_write;

    std::deque<std::string> merge_files;

    SplitHash(char*, void*, void*);
    ~SplitHash();
    void execute();

};

