#pragma once
#include "include_types.h"
#include "FileReader.h"

class ArrayStringMap
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    char* filename;
    FileReader *FR;

    ArrayStringMap();
    ArrayStringMap(void*, void*, char*);
    ~ArrayStringMap();

    char* next(uint64);

private:
    void load();
    uint32 size_of_start_offset();
};

