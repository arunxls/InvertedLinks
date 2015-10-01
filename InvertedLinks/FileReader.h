#pragma once

#include <windows.h>
#include <string>
#include "include_types.h"

class FileReader
{
public:
    char* filename;
    uint64 offset_overall;
    uint32 offset_current_read;
    uint64 size;

    void DisplayError(LPTSTR lpszFunction);

    FileReader(char* filename);
    FileReader(std::string file);
    ~FileReader();

    void read(LPVOID buffer, uint32 bytesTotransfer, uint32& bytesTrasferred);
    bool has_next();
    LONGLONG getFileSize();
    void reduceOffset(uint32);

private:
    void readFile(char*, LPVOID, OVERLAPPED&, uint32&, uint32);
    HANDLE getFileHandle();
};

