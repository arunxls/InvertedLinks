#include "stdafx.h"
#include "ArrayStringPointer.h"
#include <algorithm>

ArrayStringPointer::ArrayStringPointer()
{
}

ArrayStringPointer::ArrayStringPointer(void *start, void *end)
{
    this->start = (StringCount**)start;
    this->end = (StringCount**)end;

    this->start_offset = this->start;
    this->end_offset = this->start;
}


ArrayStringPointer::~ArrayStringPointer()
{
}

void ArrayStringPointer::put(char *pointer)
{
    *this->start_offset = (StringCount*) pointer;
    this->start_offset++;
}

struct less_than_count
{
    inline bool operator() (StringCount*& struct1, StringCount*& struct2)
    {
        return (struct1->count > struct2->count);
    }
};

void ArrayStringPointer::sort()
{
    std::sort(this->start, this->start_offset, less_than_count());
}
