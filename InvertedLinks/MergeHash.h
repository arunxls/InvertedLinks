#pragma once
#define NOMINMAX

#include <deque>
#include <string>
#include "ArrayHashCountReader.h"

#define MERGESIZE 32

class MergeHash
{
public:
    std::deque<std::string> merge_files;
    char* start;
    char* end;
    
    ArrayHashCountReader* read_1;
    ArrayHashCountReader* read_2;
    ArrayHashCountReader* write_merged;

    MergeHash(void*, void*, std::deque<std::string>);
    ~MergeHash();

    void execute();

private:
    uint32 file_count;
    std::string getNewOutputFile();
};

