#pragma once
class ArrayStringCount
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    ArrayStringCount();
    ArrayStringCount(void*, void*);
    ~ArrayStringCount();
};

