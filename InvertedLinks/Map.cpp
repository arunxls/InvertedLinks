#include "stdafx.h"
#include "Map.h"
#include "Map.h"
#include "Map.h"
#include "string.h"
#include <iostream>

Map::Map(void *start, void *end, std::string merge_file, char* map_file)
{
    this->start = (char*)start;
    this->end = (char*)end;

    this->hashCount = ArrayHashCountReader(this->start, this->start + COUNTSIZE*_1_MB);
    this->merge_file = merge_file;
    
    this->stringMap = ArrayStringMap(this->start + COUNTSIZE*_1_MB, this->start + 2*COUNTSIZE*_1_MB, map_file);

    this->stringPointer = ArrayStringPointer(this->start + 2 * COUNTSIZE*_1_MB, this->start + 2 * COUNTSIZE*_1_MB + POINTERSIZE*_1_MB);

    uint32 total_buffer_size = this->end - this->start;
    uint32 offset = (total_buffer_size - 2 * COUNTSIZE*_1_MB - POINTERSIZE*_1_MB) / 2;
    this->stringCountOriginal = ArrayStringCount(this->stringPointer.end, (char*)this->stringPointer.end + offset);
    this->stringCountSorted = ArrayStringCount((char*)this->stringPointer.end + offset, this->end);

    this->stringCountOriginal.stringPointer = &this->stringPointer;
    this->stringCountOriginal.copy = &this->stringCountSorted;
}

Map::~Map()
{
}

void Map::execute()
{
    FileReader *FR = new FileReader(this->merge_file);
    this->hashCount.setFileReader(FR);

    while (this->hashCount.has_next()) {
        HashCount h = this->hashCount.next();
        char *string = this->stringMap.next(h.hash);
        //std::cout << string << " " << h.count <<std::endl;
        this->stringCountOriginal.put(string, h.count);
    }

    return;
}
