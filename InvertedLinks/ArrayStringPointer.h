#pragma once
class ArrayStringPointer
{
public:
    char* start;
    char* end;

    char *start_offset;
    char *end_offset;

    ArrayStringPointer();
    ArrayStringPointer(void*, void*);
    ~ArrayStringPointer();
};

