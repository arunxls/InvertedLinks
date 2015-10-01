#pragma once
#include "include_types.h"
#include "FileWriter.h"
#include <vector>
#include <string>

class ArrayHashCountReader
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    std::vector<std::string> output_files;

    ArrayHashCountReader() :start(), end(), start_offset(), end_offset(), output_files() {};
    ArrayHashCountReader(void*, void*);
    ~ArrayHashCountReader();

    void put_split_files(HashCount&);
    void sort();
    void compact();
    void writeToDisk(std::string);

private:
    uint32 file_count;
    std::string getNewOutputFile();
};

