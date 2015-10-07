#include "stdafx.h"
#include "ArrayStringMap.h"

ArrayStringMap::ArrayStringMap()
{
}


ArrayStringMap::ArrayStringMap(void *start, void *end, char *filename)
{
    this->start = (char*)start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;

    this->filename = filename;
    this->FR = new FileReader(this->filename);
}

ArrayStringMap::~ArrayStringMap()
{
    delete this->FR;
}

char * ArrayStringMap::next(uint64 hash)
{
    if (this->start_offset + size_of_start_offset() >= this->end_offset) {
        this->load();
    }

    HeaderMap* hm = (HeaderMap*) this->start_offset;
    while(hm->hash != hash) {
        this->start_offset += size_of_start_offset();
        return this->next(hash);
    }

    /*char* ret = new char[hm->len];
    strcpy(ret, (char*) (hm + 1));*/

    char* ret = (char*)(hm + 1);
    this->start_offset += size_of_start_offset();
    return ret;
}

void ArrayStringMap::load()
{
    uint32 bytesTransferred = 0;
    this->FR->reduceOffset(this->end_offset - this->start_offset);
    this->FR->read(this->start, this->end - this->start, bytesTransferred);
    this->start_offset = this->start;
    this->end_offset = this->start_offset + bytesTransferred;
}

uint32 ArrayStringMap::size_of_start_offset()
{   
    HeaderMap* check = (HeaderMap*) this->start_offset;
    return sizeof(HeaderMap) + check->len;
}
