#include "stdafx.h"
#include "ArrayGraphReader.h"

ArrayGraphReader::ArrayGraphReader(void * start, void * end, FileReader* FH)
{
    this->start = (char*) start;
    this->end = (char*) end;
    
    this->start_offset = this->start;
    this->end_offset = this->start;

    this->FH = FH;
}

ArrayGraphReader::~ArrayGraphReader()
{
}

bool ArrayGraphReader::has_next()
{
    return FH->has_next() ? true : this->start_offset < this->end_offset;
}

HashCount ArrayGraphReader::next()
{
    //If start_offset and end_offset are the same, then
    //we have reached the end of our array and will need to load
    //the file again
    if (start_offset == end_offset) {
        this->load();
    }
    
    HashCount h;
    h.hash = *((uint64*)this->start_offset);
    h.count = 1;

    //Update start_offset
    this->start_offset += sizeof(uint64);

    return h;
}

void ArrayGraphReader::load()
{
    uint32 bytesTransferred = 0;
    this->FH->read(start, end - start, bytesTransferred);
    _tprintf(TEXT("Data read from %s (%d bytes): \n"), this->FH->filename, this->FH->offset_current_read);

    this->start_offset = start;
    this->end_offset = start + bytesTransferred;

    uint64* hash_start = (uint64*)start_offset;
    uint64  hash_index = 0;
    uint64  local_offset = 0;

    while (local_offset < bytesTransferred - sizeof(HeaderGraph))
    {
        // the header fits in the buffer, so we can read it
        HeaderGraph *hg = (HeaderGraph *)(this->start_offset + local_offset);
        int add = sizeof(HeaderGraph) + (hg->len * sizeof(uint64));
        if (local_offset + add <= bytesTransferred)
        {
            local_offset += add;
            //printf("Node %I64u, degree %d\n", hg->hash, hg->len);
            uint64 *neighbors = (uint64*)(hg + 1);
            uint32 len = hg->len; //save now since we are overwriting location
            for (int i = 0; i < len; i++) {
                hash_start[hash_index++] = neighbors[i];
                //printf("%I64u\n", neighbors[i]);
            }
        }
        else
        {
            //printf("Missing Node %I64u, degree %d\n", hg->hash, hg->len);
            break;
        }
    }

    this->end_offset = this->start_offset + hash_index*sizeof(uint64);
    this->FH->reduceOffset(bytesTransferred - local_offset);
}
