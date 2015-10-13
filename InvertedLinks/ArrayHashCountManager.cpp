#include "stdafx.h"
#include "ArrayHashCountManager.h"

ArrayHashCountManager::ArrayHashCountManager(void *start, void *end)
{
    this->start = (char*) start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;
    
    uint32 splitSize = (this->end - this->start) / 4;
    this->hashCount = new ArrayHashCountReader[4];
    for (int i = 0; i < 4; i++) {
        ArrayHashCountReader* tmp = this->hashCount + i;
        tmp->init(this->start + (i)*splitSize, this->start + (i + 1)*splitSize);
    }
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

}

void ArrayHashCountManager::sort()
{
}

void ArrayHashCountManager::compact()
{
}

void ArrayHashCountManager::writeToDisk(FileWriter * FH)
{
}

std::string ArrayHashCountManager::getNewOutputFile()
{
    return std::string();
}
