#pragma once
#include "include_types.h"
#include <windows.h>
#include <string>

class FileWriter
{
public:
    char* filename;

    void DisplayError(LPTSTR lpszFunction);

    FileWriter(char*);
    FileWriter(std::string&);
    ~FileWriter();

    void write(void* start, uint32 bytesToWrite);
};

