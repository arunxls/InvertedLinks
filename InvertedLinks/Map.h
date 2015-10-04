#pragma once
#include "include_types.h"
#include "ArrayHashCountReader.h"
#include "ArrayStringCount.h"
#include "ArrayStringPointer.h"
#include "ArrayStringMap.h"
#include <string>

#define COUNTSIZE 32
#define POINTERSIZE 32

class Map
{
public:
    char* start;
    char* end;

    ArrayHashCountReader hashCount;
    ArrayStringCount stringCountOriginal;
    ArrayStringCount stringCountSorted;
    ArrayStringPointer stringPointer;
    ArrayStringMap stringMap;

    Map(void*, void*, std::string, char*);
    ~Map();

    void execute();
};

