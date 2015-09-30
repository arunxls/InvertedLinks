#pragma once
#include "include_types.h"

class ArrayGraphReader
{
public:
    HeaderGraph* start;
    HeaderGraph* end;

    HeaderGraph* start_offset;
    HeaderGraph* end_offset;

    ArrayGraphReader(void*, void*);
    ~ArrayGraphReader();
};

