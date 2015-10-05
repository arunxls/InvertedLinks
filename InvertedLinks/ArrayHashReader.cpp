#include "stdafx.h"
#include "ArrayHashReader.h"


ArrayHashReader::ArrayHashReader(void* start, void* end)
{
    this->start = (uint64*)start;
    this->end = (uint64*)end;

    this->start_offset = this->start;
    this->end = this->end;
}

ArrayHashReader::ArrayHashReader(ArrayGraphReader & graph)
{
    this->start = (uint64*)graph.start;
    this->end = (uint64*)graph.end;

    this->start_offset = (uint64*)graph.start_offset;
    this->end_offset = (uint64*)graph.end_offset;
}



ArrayHashReader::~ArrayHashReader()
{
}
