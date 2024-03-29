#include "stdafx.h"
#include "ArrayStringCount.h"
#include "FileWriter.h"
//#include <stdlib.h>
#include "printf.h"

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
    this->record_count = 0;

    this->total_read = 0;
    this->total_write = 0;
}


ArrayStringCount::~ArrayStringCount()
{
}

void ArrayStringCount::put(char *string, uint32 count)
{
    int length = strlen(string) + 1;
    if (this->start_offset + length + sizeof(StringCount) > this->end) {
        this->sortCopyWrite();
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
void ArrayStringCount::load()
{
    this->FR->reduceOffset(this->end_offset - this->start_offset);
    uint32 bytesTransferred = 0;
    this->FR->read(this->start, this->end - this->start, bytesTransferred);
    this->total_read += bytesTransferred;
    this->start_offset = this->start;
    this->end_offset = this->start_offset + bytesTransferred;
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

void ArrayStringCount::sortCopyWrite()
{
    this->stringPointer->sort();
    this->copySorted();

    std::string file = this->getNewOutputFile();
    this->output_files.push_back(file);
    this->copy->writeToDisk(file);
    
    this->start_offset = this->start;
    this->copy->start_offset = this->copy->start;
    this->stringPointer->start_offset = this->stringPointer->start;
}

void ArrayStringCount::writeToDisk(std::string& file_name)
{
    WaitForSingleObject(gHashWriteSemaphone, INFINITE);
    {
        FileWriter FW = FileWriter(file_name);
        FW.write(this->start, this->start_offset - this->start);
        this->total_write += (this->start_offset - this->start);
        this->start_offset = this->start;
    }
    ReleaseSemaphore(gHashWriteSemaphone, 1, NULL);
}

void ArrayStringCount::putSingleFile(StringCount * str, std::string& file)
{
    if (this->start_offset + sizeof(StringCount) + str->len > this->end) {
        this->writeToDisk(file);
    }

    StringCount *write = (StringCount*) this->start_offset;
    write->count = str->count;
    write->len = str->len;
    this->start_offset += sizeof(StringCount);
    strcpy(this->start_offset, (char*) (str + 1));
    this->start_offset += write->len;
}

void ArrayStringCount::setFileReader(FileReader *FR)
{
    this->FR = FR;
}

StringCount * ArrayStringCount::next()
{
    if (this->start_offset + sizeof(StringCount) + ((StringCount*)this->start_offset)->len > this->end_offset) {
        this->load();
    }

    StringCount* ret = (StringCount*) this->start_offset;
    this->start_offset += sizeof(StringCount) + ret->len;
    return ret;
}

bool ArrayStringCount::has_next()
{
    return this->FR->has_next() ? true : (this->start_offset < this->end_offset);
}

StringCount* ArrayStringCount::current()
{
    if (this->start_offset + sizeof(StringCount) + ((StringCount*) this->start_offset)->len > this->end_offset) {
        this->load();
    }
    return (StringCount*) this->start_offset;
}

void ArrayStringCount::putFinalRun(StringCount * str, std::string& file)
{
    this->record_count++;
    //sparetimelabs.com/printfrevisited
    sprintf(this->start_offset, "%d %s %d\r\n",this->record_count, (char*)(str + 1), str->count);
    uint32 length = strlen(this->start_offset);
    if (this->start_offset + length > this->end) {
        this->writeToDisk(file);
        //sparetimelabs.com/printfrevisited
        sprintf(this->start_offset, "%d %s %d\r\n", this->record_count, (char*)(str + 1), str->count);
    }
    this->start_offset += length;
}

std::string ArrayStringCount::getNewOutputFile()
{
    return STRING_OUTPUT_PREFIX + std::to_string(this->file_count++) + STRING_OUTPUT_SUFFIX;
}