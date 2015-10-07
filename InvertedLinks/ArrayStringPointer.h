#pragma once
#include "include_types.h"

class ArrayStringPointer
{
public:
    StringCount** start;
    StringCount** end;

    StringCount** start_offset;
    StringCount** end_offset;

    uint64 total_read;
    uint64 total_write;

    ArrayStringPointer();
    ArrayStringPointer(void*, void*);
    ~ArrayStringPointer();

    void put(char*);
    void sort();
};

