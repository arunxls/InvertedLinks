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

    this->hashCount = new ArrayHashCountReader(this->start, this->start + COUNTSIZE*_1_MB);
    this->merge_file = merge_file;
    
    this->stringMap = new ArrayStringMap(this->start + COUNTSIZE*_1_MB, this->start + 2*COUNTSIZE*_1_MB, map_file);

    this->stringPointer = new ArrayStringPointer(this->start + 2 * COUNTSIZE*_1_MB, this->start + 2 * COUNTSIZE*_1_MB + POINTERSIZE*sizeof(char*)*_1_MB);

    uint32 total_buffer_size = this->end - this->start;
    uint32 offset = (total_buffer_size - 2 * COUNTSIZE*_1_MB - POINTERSIZE*sizeof(char*)*_1_MB) / 2;
    this->stringCountOriginal = new ArrayStringCount(this->stringPointer->end, (char*)this->stringPointer->end + offset);
    this->stringCountSorted = new ArrayStringCount((char*)this->stringPointer->end + offset, this->end);

    this->stringCountOriginal->stringPointer = this->stringPointer;
    this->stringCountOriginal->copy = this->stringCountSorted;

    this->total_read = 0;
    this->total_write = 0;
}

Map::~Map()
{
    delete this->stringCountOriginal;
    delete this->stringCountSorted;
    delete this->stringMap;
    delete this->hashCount;
    delete this->stringPointer;
}

void Map::execute()
{
    FileReader *FR = new FileReader(this->merge_file);
    printf("Total unique nodes - %I32u\n", FR->size / sizeof(HashCount));
    this->hashCount->setFileReader(FR);

    while (this->hashCount->has_next()) {
        HashCount h = this->hashCount->next();
        char *string = this->stringMap->next(h.hash);
        this->stringCountOriginal->put(string, h.count);
    }
    this->stringCountOriginal->sortCopyWrite();
    this->output_files = this->stringCountOriginal->output_files;

    delete FR;
    DeleteFile(TEXT(this->merge_file.c_str()));

    this->total_read +=
        this->stringCountOriginal->total_read +
        this->stringCountSorted->total_read +
        this->stringPointer->total_read +
        this->stringMap->total_read +
        this->hashCount->total_read;

    this->total_write +=
        this->stringCountOriginal->total_write +
        this->stringCountSorted->total_write +
        this->stringPointer->total_write +
        this->stringMap->total_write +
        this->hashCount->total_write;

    return;
}
