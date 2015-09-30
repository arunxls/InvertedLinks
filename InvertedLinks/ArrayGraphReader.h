#pragma once
#include "include_types.h"
#include "FileReader.h"

class ArrayGraphReader
{
public:
    HeaderGraph* start;
    HeaderGraph* end;

    HeaderGraph* start_offset;
    HeaderGraph* end_offset;

    FileReader* FH;

    ArrayGraphReader(void*, void*, FileReader*);
    ~ArrayGraphReader();
};

