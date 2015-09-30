#pragma once
#include "include_types.h"
#include "ArrayGraphReader.h"

class ArrayHashReader
{
public:
    uint64* start;
    uint64* end;

    uint64* start_offset;
    uint64* end_offset;

    ArrayHashReader(void*, void*);
    ArrayHashReader(ArrayGraphReader& graph);
    ~ArrayHashReader();
};

