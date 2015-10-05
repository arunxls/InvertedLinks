#pragma once
#include "include_types.h"
#include "FileWriter.h"
#include "FileReader.h"
#include <deque>
#include <string>

class ArrayHashCountReader
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;

    std::deque<std::string> output_files;

    ArrayHashCountReader() :start(), end(), start_offset(), end_offset(), output_files() {};
    ArrayHashCountReader(void*, void*);
    ~ArrayHashCountReader();

    void putSplitFiles(HashCount&);
    void putSingleFile(HashCount&);
    void sort();
    void compact();
    void writeToDisk(std::string);
    bool has_next();
    HashCount& current();
    HashCount next();
    void load();
    void setFileReader(FileReader*);
    void setFileWriter(FileWriter*);
    std::string getNewOutputFile();

private:
    uint32 file_count;
    FileReader* FR;
    FileWriter* FW;
};

