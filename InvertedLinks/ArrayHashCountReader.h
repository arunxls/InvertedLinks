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

    uint64 total_read;
    uint64 total_write;

    std::deque<std::string> output_files;

    ArrayHashCountReader();
    ArrayHashCountReader(void*, void*);
    ~ArrayHashCountReader();

    void putSplitFiles(HashCount&);
    void putSingleFile(HashCount&, std::string&);
    void sort();
    void compact();
    void writeToDisk(std::string& file_name);
    bool has_next();
    HashCount& current();
    HashCount next();
    void load();
    void setFileReader(FileReader*);
    std::string getNewOutputFile();
    void init(void*, void*, int);

private:
    int thread_count;
    uint32 file_count;
    FileReader* FR;
};

