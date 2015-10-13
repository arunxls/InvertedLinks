#include "stdafx.h"
#include "ArrayHashCountManager.h"

ArrayHashCountManager::ArrayHashCountManager(void *start, void *end)
{
    this->file_count = 1;
    this->start = (char*) start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;
    
    uint32 splitSize = (this->end - this->start) / 4;
    this->hashCount = new ArrayHashCountReader[MAX_SPLIT_THREADS];
    for (int i = 0; i < MAX_SPLIT_THREADS; i++) {
        ArrayHashCountReader* tmp = this->hashCount + i;
        tmp->init(this->start + (i)*splitSize, this->start + (i + 1)*splitSize);
    }

    this->total_read = 0;
    this->total_write = 0;
}

ArrayHashCountManager::ArrayHashCountManager()
{
}


ArrayHashCountManager::~ArrayHashCountManager()
{
    delete[] this->hashCount;
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

DWORD WINAPI sortAndCompact(LPVOID data)
{
    ArrayHashCountReader* hCount = (ArrayHashCountReader*)data;
    hCount->sort();
    hCount->compact();
    return 0;
}

void ArrayHashCountManager::writeToDisk(FileWriter * FH)
{
    DWORD   dwThreadIdArray[MAX_SPLIT_THREADS];
    HANDLE  hThreadArray[MAX_SPLIT_THREADS];

    for (int i = 0; i < MAX_SPLIT_THREADS; ++i) {
        hThreadArray[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            sortAndCompact,   // thread function name
            (this->hashCount + i),  // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 

        if (hThreadArray[i] == NULL)
        {
            //ErrorHandler(TEXT("CreateThread"));
            ExitProcess(3);
        }
    }

    WaitForMultipleObjects(MAX_SPLIT_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles.
    for (int i = 0; i<MAX_SPLIT_THREADS; ++i)
    {
        CloseHandle(hThreadArray[i]);
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
