#include "stdafx.h"
#include "ArrayStringCount.h"
#include "FileWriter.h"

ArrayStringCount::ArrayStringCount()
{
}

ArrayStringCount::ArrayStringCount(void *start, void *end)
{
    this->start = (char*)start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;

    this->file_count = 1;
}


ArrayStringCount::~ArrayStringCount()
{
}

void ArrayStringCount::put(char *string, uint32 count)
{
    int length = strlen(string) + 1;
    if (this->start_offset + length + sizeof(StringCount) > this->end) {
        this->writeToDisk();
    }

    this->stringPointer->put(this->start_offset);
    
    StringCount* strCount = (StringCount*) this->start_offset;
    //Add count
    strCount->count = count;
    strCount->len = length;
    this->start_offset += sizeof(StringCount);
    
    //Add string
    strcpy(this->start_offset, string);
    this->start_offset += length;
}

void ArrayStringCount::put(StringCount* toCopy)
{
    StringCount* strCount = (StringCount*) this->start_offset;
    //Add count
    strCount->count = toCopy->count;
    strCount->len = toCopy->len;
    this->start_offset += sizeof(StringCount);

    //Add string
    strcpy(this->start_offset, (char*) (toCopy + 1));
    this->start_offset += toCopy->len;
}
void ArrayStringCount::copySorted()
{
    StringCount** current = this->stringPointer->start;
    while (current < this->stringPointer->start_offset) {
        StringCount* elem = *current;
        this->copy->put(*current);
        current++;
    }
}

void ArrayStringCount::writeToDisk()
{
    this->stringPointer->sort();
    this->copySorted();

    std::string file = this->getNewOutputFile();
    FileWriter FW = FileWriter(file);
    FW.write(this->start, this->start_offset - this->start);
    
    this->start_offset = this->start;
    this->copy->start_offset = this->copy->start;
    this->stringPointer->start_offset = this->stringPointer->start;
}

std::string ArrayStringCount::getNewOutputFile()
{
    return STRING_OUTPUT_PREFIX + std::to_string(this->file_count++) + STRING_OUTPUT_SUFFIX;;
}