#pragma once
#include "include_types.h"
#include "ArrayStringPointer.h"
#include <string>
#include <deque>

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
    std::deque<std::string> output_files;

    FileReader* FR;
    FileWriter* FW;

    ArrayStringCount();
    ArrayStringCount(void*, void*);
    ~ArrayStringCount();

    void put(char*, uint32);
    void put(StringCount*);
    void writeToDisk();
    void writeToDisk(std::string);
    void putSingleFile(StringCount&);
    void setFileWriter(FileWriter*);
    void setFileReader(FileReader*);
    StringCount& next();
    bool has_next();
    StringCount& current();

private:
    void copySorted();
    std::string getNewOutputFile();
};

