#pragma once

#include <vector>
#include <string>
#include "ArrayHashCountReader.h"

#define MERGESIZE 32

class MergeHash
{
public:
    std::vector<std::string> merge_files;
    char* start;
    char* end;
    
    ArrayHashCountReader read_1;
    ArrayHashCountReader read_2;
    ArrayHashCountReader write_merged;


    MergeHash(char*, char*, std::vector<std::string>);
    ~MergeHash();
};

