#pragma once
#include <string>
#include <deque>
#include "ArrayStringCount.h"

class MapMerge
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    int file_count;

    uint64 total_read;
    uint64 total_write;

    ArrayStringCount* merge_1;
    ArrayStringCount* merge_2;
    ArrayStringCount* output;

    std::deque<std::string> merge_files;
    MapMerge();
    MapMerge(void*, void*, std::deque<std::string>&);
    ~MapMerge();

    void execute();
    void executeFinal();
    void init(void*, void*, std::deque<std::string>&);

private:
    std::string getNewOutputFile();
};

