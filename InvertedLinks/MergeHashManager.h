#pragma once
#include "include_types.h"
#include "MergeHash.h"

#define MAX_MERGE_THREADS 2

class MergeHashManager
{
public:
    std::deque<std::string> merge_files;
    char* start;
    char* end;

    uint64 total_read;
    uint64 total_write;

    MergeHashManager(void*, void*, std::deque<std::string>);
    MergeHashManager();
    ~MergeHashManager();

    void execute();
private:
    uint32 file_count;
};

