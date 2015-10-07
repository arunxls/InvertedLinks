#include "stdafx.h"
#include "SplitHash.h"
#include "FileReader.h"

SplitHash::SplitHash(char *filename, void *buffer_start, void *buffer_end) :
    buffer_start((char*)buffer_start), buffer_end((char*)buffer_end) 
{
    this->FR = new FileReader(filename);
    this->graph = new ArrayGraphReader(buffer_start, (char*)buffer_start + READBUFFER*_1_MB, FR);
    uint32 offset = ((uint32)std::ceil( ( (float) READBUFFER*_1_MB) / ( (float) sizeof(HashCount) ) )) * sizeof(HashCount) ;
    this->hashCount = new ArrayHashCountReader((char*)buffer_start + offset, buffer_end);

    this->total_read = 0;
    this->total_write = 0;
}

SplitHash::~SplitHash()
{
    delete this->FR;
    delete this->graph;
    delete this->hashCount;
}

void SplitHash::execute()
{
    int count = 0;
    while (this->graph->has_next()) {
        HashCount h = this->graph->next();
        this->hashCount->putSplitFiles(h);
    }

    this->hashCount->sort();
    this->hashCount->compact();
    std::string file_name = this->hashCount->getNewOutputFile();
    FileWriter FW = FileWriter(file_name);
    this->hashCount->writeToDisk(&FW);

    this->merge_files = this->hashCount->output_files;

    this->total_read += this->graph->total_read + this->hashCount->total_read;
    this->total_write += this->graph->total_write + this->hashCount->total_write;
}
