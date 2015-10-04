#pragma once
#include "include_types.h"

class ArrayStringMap
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    char* filename;

    ArrayStringMap();
    ArrayStringMap(void*, void*, char*);
    ~ArrayStringMap();

    char* next(uint64);

private:
    void load();
};

