#pragma once
#include "include_types.h"
#include <deque>

#define MAX_MAP_MERGE_THREADS 2

class MapMergeManager
{
public:
    std::deque<std::string> merge_files;
    char* start;
    char* end;

    uint64 total_read;
    uint64 total_write;

    MapMergeManager();
    ~MapMergeManager();
private:
    uint32 file_count;
    std::string getNewOutputFile();
};

