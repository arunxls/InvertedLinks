#pragma once
#include "include_types.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "ArrayHashCountReader.h"

#define MAX_SPLIT_THREADS 4

class ArrayHashCountManager
{
public:
    char* start;
    char* end;

    char* start_offset;
    char* end_offset;
    std::deque<std::string> output_files;

    uint64 total_read;
    uint64 total_write;

    ArrayHashCountReader* hashCount;

    ArrayHashCountManager(void*, void*);
    ArrayHashCountManager();
    ~ArrayHashCountManager();

    void putSplitFiles(HashCount&);
    void writeToDisk(std::string&);
    std::string getNewOutputFile();
    uint64 getTotalRead();
    uint64 getTotalWrite();

private:
    HANDLE  hThreadArray[MAX_SPLIT_THREADS];
    uint32 file_count;
    void init_threads();
};

