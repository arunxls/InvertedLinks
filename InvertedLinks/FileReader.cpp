#include "stdafx.h"
#include <windows.h>
#include "FileReader.h"
#include "include_types.h"
#include <tchar.h>
#include <cstdio>
#include <strsafe.h>
#include <csignal>
#include <cstdlib>

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped)
{
    if (DEBUG)
    {
        _tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
        _tprintf(TEXT("Number of bytes:\t%I32u\n"), dwNumberOfBytesTransfered);
    }
}

void FileReader::DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);

    lpDisplayBuf =
        (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf)
                + lstrlen((LPCTSTR)lpszFunction)
                + 40) // account for format string
            * sizeof(TCHAR));

    if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error code %d as follows:\n%s"),
        lpszFunction,
        dw,
        lpMsgBuf)))
    {
        printf("FATAL ERROR: Unable to output error code.\n");
    }

    _tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

FileReader::FileReader(char*& filename)
{
    this->filename = filename;
    this->offset_current_read = 0;
    this->offset_overall = 0;
 
    this->hFile = this->getFileHandle();
    this->size = this->getFileSize();
}

FileReader::FileReader(std::string& file)
{
    this->filename = &file[0u];
    this->offset_current_read = 0;
    this->offset_overall = 0;

    this->hFile = this->getFileHandle();
    this->size = this->getFileSize();
}

FileReader::~FileReader()
{
    CloseHandle(this->hFile);
}

void FileReader::read(LPVOID buffer, uint32 bytesTotransfer, uint32& bytesTrasferred)
{
    OVERLAPPED ol = { 0 };

    ol.OffsetHigh = (uint32)(offset_overall >> 32);
    ol.Offset = (uint32)offset_overall;
    ol.hEvent = *(HANDLE*) this;

    this->readFile(filename, buffer, ol, bytesTrasferred, bytesTotransfer);
    
    this->offset_current_read = bytesTrasferred;
    this->offset_overall += bytesTrasferred;

    return;
}

bool FileReader::has_next()
{
    return this->offset_overall < this->size;
}

LONGLONG FileReader::getFileSize()
{
    LARGE_INTEGER size;
    GetFileSizeEx(this->hFile, &size);
    return size.QuadPart;
}

void FileReader::reduceOffset(uint32 reduction)
{
    this->offset_overall -= reduction;
    this->offset_current_read -= reduction;
}

void FileReader::readFile(char* filename, LPVOID buffer, OVERLAPPED& ol, uint32& dwBytesRead, uint32 bufferSize) {
    if (DEBUG)
    {
        printf("Reading %s\n", filename);
    }

    bool flag = ReadFileEx(this->hFile, (char*)buffer, bufferSize, &ol, FileIOCompletionRoutine);
    if (DEBUG && !flag)
    {
        this->DisplayError(TEXT("ReadFile"));
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        return;
    }
    SleepEx(50000, TRUE);
    dwBytesRead = ol.InternalHigh;
}

HANDLE FileReader::getFileHandle()
{
    HANDLE hFile = CreateFile(this->filename,               // file to open
        GENERIC_READ,          // open for reading
        0,       // exclusive reading
        NULL,                  // default security
        OPEN_EXISTING,         // existing file only
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        hFile = CreateFile(this->filename,               // file to open
            GENERIC_READ,          // open for reading
            FILE_SHARE_READ,       // share for reading
            NULL,                  // default security
            OPEN_EXISTING,         // existing file only
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
            NULL);
        if (DEBUG && hFile == INVALID_HANDLE_VALUE) {
            _tprintf("Terminal failure: unable to open file \"%s\" for read.\n", this->filename);
            std::abort();
        }
    }

    return hFile;
}