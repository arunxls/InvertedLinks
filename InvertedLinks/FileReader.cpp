#include "stdafx.h"
#include <windows.h>
#include "FileReader.h"
#include "include_types.h"
#include "file_manager.h"

FileReader::FileReader(char* filename)
{
    this->filename = filename;
    this->offset_current_read = 0;
    this->offset_overall = 0;
}

FileReader::FileReader(std::string file)
{
    this->filename = &file[0u];
    this->offset_current_read = 0;
    this->offset_overall = 0;
}

FileReader::~FileReader()
{
}

void FileReader::read(void * buffer, uint32 bytesTotransfer, uint32 & bytesTrasferred)
{
    uint32 dwBytesRead = 0;
    OVERLAPPED ol = { 0 };

    ol.OffsetHigh = (uint32)(offset_overall >> 32);
    ol.Offset = (uint32)offset_overall;

    readFile(filename, buffer, ol, dwBytesRead, bytesTotransfer);
    
    this->offset_current_read = dwBytesRead;
    this->offset_overall += dwBytesRead;

    return;
}
