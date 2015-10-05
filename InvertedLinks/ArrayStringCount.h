#pragma once
#include "include_types.h"
#include "ArrayStringPointer.h"
class ArrayStringCount
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    ArrayStringCount* copy;
    ArrayStringPointer* stringPointer;

    uint32 file_count;

    ArrayStringCount();
    ArrayStringCount(void*, void*);
    ~ArrayStringCount();

    void put(char*, uint32);
    void put(StringCount*);

private:
    void copySorted();
    void writeToDisk();
    std::string getNewOutputFile();
};

