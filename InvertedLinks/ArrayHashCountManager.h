#pragma once
#include "include_types.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "ArrayHashCountReader.h"

class ArrayHashCountManager
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    uint64 total_read;
    uint64 total_write;

    ArrayHashCountReader* hashCount;

    ArrayHashCountManager(void*, void*);
    ArrayHashCountManager();
    ~ArrayHashCountManager();

    void putSplitFiles(HashCount&);
    void sort();
    void compact();
    void writeToDisk(FileWriter* FH);
    std::string getNewOutputFile();
};

