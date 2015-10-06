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

    ArrayStringCount merge_1;
    ArrayStringCount merge_2;
    ArrayStringCount output;

    std::deque<std::string> merge_files;
    MapMerge();
    MapMerge(void*, void*, std::deque<std::string>);
    ~MapMerge();

    void execute();
private:
    std::string getNewOutputFile();
};

