#include "stdafx.h"
#include "ArrayHashCountManager.h"

HANDLE ghSortSemaphore;
HANDLE ghWriteSemaphore;

ArrayHashCountManager::ArrayHashCountManager(void *start, void *end)
{
    this->file_count = 1;
    this->start = (char*) start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;
    
    ghSortSemaphore = CreateSemaphore(NULL,0,MAX_SPLIT_THREADS,NULL);
    ghWriteSemaphore = CreateSemaphore(NULL, 0, MAX_SPLIT_THREADS, NULL);

    uint32 splitSize = (this->end - this->start) / MAX_SPLIT_THREADS;
    this->hashCount = new ArrayHashCountReader[MAX_SPLIT_THREADS];
    for (int i = 0; i < MAX_SPLIT_THREADS; i++) {
        ArrayHashCountReader* tmp = this->hashCount + i;
        tmp->init(this->start + (i)*splitSize, this->start + (i + 1)*splitSize);
    }

    this->total_read = 0;
    this->total_write = 0;
    this->init_threads();
}

ArrayHashCountManager::ArrayHashCountManager()
{
}


ArrayHashCountManager::~ArrayHashCountManager()
{
    delete[] this->hashCount;
    CloseHandle(ghSortSemaphore);
    CloseHandle(ghWriteSemaphore);
    for (int i = 0; i < MAX_SPLIT_THREADS; ++i) {
        CloseHandle(this->hThreadArray[i]);
    }
}

void ArrayHashCountManager::putSplitFiles(HashCount &h)
{
    int key = ((h.hash & 0xc000000000000000) >> 62);
    ArrayHashCountReader* tmp = (this->hashCount + key);
    if (tmp->start_offset + sizeof(HashCount) > tmp->end) {
        std::string file = this->getNewOutputFile();
        FileWriter* FW = new FileWriter(file);
        this->writeToDisk(FW);
        delete FW;
    }
    tmp->putSplitFiles(h);
}

void DisplayError(LPTSTR lpszFunction)
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

DWORD WINAPI sortAndCompact(LPVOID data)
{
    while (1) {
        WaitForSingleObject(ghSortSemaphore, INFINITE);
        WaitForSingleObject(ghWriteSemaphore, INFINITE);

        ArrayHashCountReader* hCount = (ArrayHashCountReader*)data;
        hCount->sort();
        hCount->compact();
        if (!ReleaseSemaphore(ghWriteSemaphore,1,NULL))
        {
            DisplayError(TEXT("SetEvent"));
            return 1;
        }
    }

    return 0;
}

void ArrayHashCountManager::writeToDisk(FileWriter * FH)
{
    DWORD   dwThreadIdArray[MAX_SPLIT_THREADS];
    HANDLE  hThreadArray[MAX_SPLIT_THREADS];

    for (int i = 0; i < MAX_SPLIT_THREADS; ++i) {
        if (!ReleaseSemaphore(ghSortSemaphore, 1, NULL))
        {
            DisplayError(TEXT("SetEvent"));
        }

        if (!ReleaseSemaphore(ghWriteSemaphore, 1, NULL))
        {
            DisplayError(TEXT("SetEvent"));
        }
    }

    Sleep(1000);
    for (int i = 0; i < MAX_SPLIT_THREADS; ++i)
    {
        WaitForSingleObject(ghWriteSemaphore, INFINITE);
    }

    for (int i = 0; i < MAX_SPLIT_THREADS; ++i)
    {
        (this->hashCount + i)->writeToDisk(FH);
    }
}



std::string ArrayHashCountManager::getNewOutputFile()
{
    std::string file = OUTPUT_PREFIX + std::to_string(this->file_count++) + OUTPUT_SUFFIX;
    this->output_files.push_back(file);
    return file;
}

uint64 ArrayHashCountManager::getTotalRead()
{
    for (int i = 0; i < MAX_SPLIT_THREADS; ++i)
    {
        this->total_read += (this->hashCount + i)->total_read;
    }
    return this->total_read;
}

uint64 ArrayHashCountManager::getTotalWrite()
{
    for (int i = 0; i < MAX_SPLIT_THREADS; ++i)
    {
        this->total_write += (this->hashCount + i)->total_write;
    }
    return this->total_write;
}

void ArrayHashCountManager::init_threads()
{
    DWORD   dwThreadIdArray[MAX_SPLIT_THREADS];
    for (int i = 0; i < MAX_SPLIT_THREADS; ++i) {
        this->hThreadArray[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            sortAndCompact,         // thread function name
            (this->hashCount + i),  // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 

        //SetThreadPriority(this->hThreadArray[i], THREAD_PRIORITY_HIGHEST);

        if (hThreadArray[i] == NULL)
        {
            //ErrorHandler(TEXT("CreateThread"));
            ExitProcess(3);
        }
    }
}
