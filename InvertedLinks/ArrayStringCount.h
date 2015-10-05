#pragma once
#include "include_types.h"
#include "ArrayStringPointer.h"

const std::string STRING_OUTPUT_PREFIX = "string";
const std::string STRING_OUTPUT_SUFFIX = "";

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
    void writeToDisk();

private:
    void copySorted();
    std::string getNewOutputFile();
};

