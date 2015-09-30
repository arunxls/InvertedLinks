#pragma once

#include <string>
#include "include_types.h"
#include <windows.h>

class FileReader
{
public:
    char* filename;
    uint32 offset_overall;
    uint32 offset_current_read;

    FileReader(char* filename);
    void read(void* buffer, uint32 bytesTotransfer, uint32& bytesTrasferred);
    FileReader(std::string file);
    ~FileReader();
};

