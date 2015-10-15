#pragma once
#define NOMINMAX

#include <deque>
#include <string>
#include "ArrayHashCountReader.h"

#define MERGESIZE 36

const std::string MERGE_OUTPUT_PREFIX = "merge";
const std::string MERGE_OUTPUT_SUFFIX = "";

class MergeHash
{
public:
    std::deque<std::string> merge_files;
    char* start;
    char* end;
    
    int thread_count;

    ArrayHashCountReader* read_1;
    ArrayHashCountReader* read_2;
    ArrayHashCountReader* write_merged;

    uint64 total_read;
    uint64 total_write;
    uint32 file_count;

    MergeHash(void*, void*, std::deque<std::string>);
    MergeHash();
    ~MergeHash();

    void init(void*, void*, std::deque<std::string>);

    void execute();

private:
    std::string getNewOutputFile();
};

