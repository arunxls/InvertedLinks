#include "stdafx.h"
#include "ArrayHashCountReader.h"
#include <algorithm>
#include "Sort.h"

ArrayHashCountReader::ArrayHashCountReader(void * start, void * end)
{
    this->init(start, end);

    this->file_count = 1;
    this->FR = NULL;

    this->total_read = 0;
    this->total_write = 0;
}

ArrayHashCountReader::ArrayHashCountReader()
{
    this->file_count = 1;
    this->FR = NULL;

    this->total_read = 0;
    this->total_write = 0;
}

ArrayHashCountReader::~ArrayHashCountReader()
{
}

void ArrayHashCountReader::putSplitFiles(HashCount& h)
{
    if (this->start_offset + sizeof(HashCount) > this->end) {
        this->sort();
        this->compact();
        std::string file_name = this->getNewOutputFile();
        FileWriter FW = FileWriter(file_name);
        this->writeToDisk(&FW);
    }

    HashCount* foo = (HashCount*) this->start_offset;
    foo->hash = h.hash;
    foo->count = h.count;

    this->start_offset += sizeof(HashCount);
}

void ArrayHashCountReader::putSingleFile(HashCount & h)
{
    if (this->start_offset + sizeof(HashCount) >= this->end) {
        this->compact();
        this->FW->write(this->start, this->start_offset - this->start);
        this->total_write += this->start_offset - this->start;
        this->start_offset = this->start;
    }

    *(HashCount*) this->start_offset = h;
    this->start_offset += sizeof(HashCount);
}

void ArrayHashCountReader::sort()
{
    std::sort((HashCount*)this->start, (HashCount*)this->start_offset);
}

void ArrayHashCountReader::compact()
{
    HashCount* insert_p = (HashCount*) this->start;
    HashCount* check_p = insert_p + 1;
    while (check_p < (HashCount*)this->start_offset) {
        if (*insert_p == *check_p) {
            *insert_p += *check_p;
        }
        else {
            insert_p++;
            *insert_p = *check_p;
        }
        check_p++;
    }

    this->start_offset = (char*) (insert_p + 1);
}

void ArrayHashCountReader::writeToDisk(FileWriter* FH)
{
    this->output_files.emplace_back(FH->filename);
    FH->write(this->start, this->start_offset - this->start);
    this->total_write += (this->start_offset - this->start);
    this->start_offset = this->start;
}

bool ArrayHashCountReader::has_next()
{
    return FR->has_next() ? true : (this->start_offset < this->end_offset);
}

HashCount ArrayHashCountReader::next()
{
    if (this->start_offset >= this->end_offset) {
        this->load();
    }
    HashCount* next = (HashCount*)(this->start_offset);
    this->start_offset += sizeof(HashCount);
    return *next;
}

void ArrayHashCountReader::load()
{
    if (this->has_next()) {
        uint32 bytesTransferred = 0;
        this->FR->read(this->start, this->end - this->start, bytesTransferred);
        this->total_read += bytesTransferred;
        this->start_offset = this->start;
        this->end_offset = this->start_offset + bytesTransferred;
    }
}

void ArrayHashCountReader::setFileReader(FileReader * FR)
{
    this->FR = FR;
}

void ArrayHashCountReader::setFileWriter(FileWriter * FW)
{
    this->FW = FW;
}

HashCount& ArrayHashCountReader::current()
{
    if (this->start_offset >= this->end_offset) {
        this->load();
    }

    return *(HashCount*)this->start_offset;
}

std::string ArrayHashCountReader::getNewOutputFile()
{
    return OUTPUT_PREFIX + std::to_string(this->file_count++) + OUTPUT_SUFFIX;
}

void ArrayHashCountReader::init(void *start, void *end)
{
    this->start = (char*)start;
    this->end = (char*)end;

    this->start_offset = this->start;
    this->end_offset = this->start;
}
