#include "stdafx.h"
#include "SplitHash.h"
#include "FileReader.h"

SplitHash::SplitHash(char *filename, void *buffer_start, void *buffer_end) :
    buffer_start((char*)buffer_start), buffer_end((char*)buffer_end) 
{
    this->FR = FileReader(filename);

    this->graph = ArrayGraphReader(buffer_start, (char*)buffer_start + READBUFFER*_1_MB, &FR);

    uint32 offset = ((uint32)std::ceil( ( (float) READBUFFER*_1_MB) / ( (float) sizeof(HashCount) ) )) * sizeof(HashCount) ;

    this->hashCount = ArrayHashCountReader((char*)buffer_start + offset, buffer_end);
}

SplitHash::~SplitHash()
{
}

void SplitHash::execute()
{
    int count = 0;
    while (this->graph.has_next()) {
        HashCount h = this->graph.next();
        this->hashCount.putSplitFiles(h);
    }

    this->hashCount.sort();
    this->hashCount.compact();
    std::string file_name = this->hashCount.getNewOutputFile();
    this->hashCount.writeToDisk(file_name);

    this->merge_files = this->hashCount.output_files;
}
