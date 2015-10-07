#pragma once
#include "include_types.h"
#include "ArrayHashCountReader.h"
#include "ArrayStringCount.h"
#include "ArrayStringPointer.h"
#include "ArrayStringMap.h"
#include <string>
#include <deque>

#define COUNTSIZE 36
#define POINTERSIZE 9 //This then gets multiplied by sizeof(char*)

class Map
{
public:
    char* start;
    char* end;

    ArrayHashCountReader* hashCount;
    ArrayStringCount* stringCountOriginal;
    ArrayStringCount* stringCountSorted;
    ArrayStringPointer* stringPointer;
    ArrayStringMap* stringMap;

    uint64 total_read;
    uint64 total_write;

    std::string merge_file;
    std::deque<std::string> output_files;

    Map(void*, void*, std::string, char*);
    ~Map();

    void execute();
};

