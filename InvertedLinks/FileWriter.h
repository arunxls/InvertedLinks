#pragma once
#include "include_types.h"
#include <windows.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <strsafe.h>
#include <csignal>
#include <cstdlib>

class FileWriter
{
public:
    char* filename;
    
    HANDLE hFile;

    void DisplayError(LPTSTR lpszFunction);

    FileWriter(char*);
    FileWriter(std::string&);
    ~FileWriter();

    void write(void* start, uint32 bytesToWrite);

private:
    HANDLE getFileHandle();
};

