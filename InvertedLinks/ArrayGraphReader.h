#pragma once
#include "include_types.h"
#include "FileReader.h"

class ArrayGraphReader
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    FileReader* FH;

    uint64 total_read;
    uint64 total_write;

    ArrayGraphReader() : start(), end(), start_offset(), end_offset() {};
    ArrayGraphReader(void*, void*, FileReader*);
    ~ArrayGraphReader();
    bool has_next();
    HashCount next();
    void load();
};

