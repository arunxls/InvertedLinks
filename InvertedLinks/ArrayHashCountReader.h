#pragma once
#include "include_types.h"

class ArrayHashCountReader
{
public:
    HashCount* start;
    HashCount* end;

    HashCount* start_offset;
    HashCount* end_offset;

    ArrayHashCountReader(void*, void*);
    ~ArrayHashCountReader();
};

