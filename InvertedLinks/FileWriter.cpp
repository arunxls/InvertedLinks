#include "stdafx.h"
#include "FileWriter.h"

void FileWriter::DisplayError(LPTSTR lpszFunction)
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

FileWriter::FileWriter(char * filename) :filename(filename)
{
    this->hFile = this->getFileHandle();
}

FileWriter::FileWriter(std::string& filename){
    char* f = &filename[0u];
    this->filename = f;
    this->hFile = this->getFileHandle();
}

FileWriter::~FileWriter()
{
    CloseHandle(this->hFile);
}

void FileWriter::write(void * start, uint32 bytesToWrite)
{
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    if (DEBUG) {
        _tprintf(TEXT("Writing %d bytes to %s.\n"), bytesToWrite, this->filename);
    }

    bErrorFlag = WriteFile(
        this->hFile,           // open file handle
        start,      // start of data to write
        bytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (DEBUG && FALSE == bErrorFlag)
    {
        DisplayError(TEXT("WriteFile"));
        printf("Terminal failure: Unable to write to file.\n");
    }
    else
    {
        if (DEBUG && dwBytesWritten != bytesToWrite)
        {
            // This is an error because a synchronous write that results in
            // success (WriteFile returns TRUE) should write all data as
            // requested. This would not necessarily be the case for
            // asynchronous writes.
            printf("Error: dwBytesWritten != dwBytesToWrite\n");
        }
        else
        {
            if (DEBUG) {
                _tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, filename);
            }
        }
    }
}

HANDLE FileWriter::getFileHandle()
{
    HANDLE hFile;
    hFile = CreateFile(
        this->filename,               // name of the write
        FILE_APPEND_DATA,          // open for writing
        FILE_SHARE_READ,                      // shared read 
        NULL,                   // default security
        OPEN_EXISTING,             // open existing file
        FILE_ATTRIBUTE_NORMAL | FILE_APPEND_DATA,  // normal file
        NULL);                  // no attr. template

    if (GetLastError() == ERROR_FILE_NOT_FOUND) {
        hFile = CreateFile(
            this->filename,               // name of the write
            GENERIC_WRITE,          // open for writing
            0,                      // shared read
            NULL,                   // default security
            CREATE_NEW,             // open existing file
            FILE_ATTRIBUTE_NORMAL,  // normal file
            NULL);
    }

    if (DEBUG && hFile == INVALID_HANDLE_VALUE)
    {
        this->DisplayError(TEXT("CreateFile"));
        _tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), filename);
        return HANDLE();
    }

    return hFile;
}
