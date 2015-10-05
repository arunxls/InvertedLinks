#pragma once
#include "include_types.h"

class ArrayStringPointer
{
public:
    StringCount** start;
    StringCount** end;

    StringCount** start_offset;
    StringCount** end_offset;

    ArrayStringPointer();
    ArrayStringPointer(void*, void*);
    ~ArrayStringPointer();

    void put(char*);
    void sort();
};

