#include "stdafx.h"
#include "SplitHash.h"
#include "FileReader.h"

SplitHash::SplitHash(char *filename, void *buffer_start, void *buffer_end) :
    buffer_start((char*)buffer_start), buffer_end((char*)buffer_end) 
{
    this->FR = new FileReader(filename);
    this->graph = new ArrayGraphReader(this->buffer_start, this->buffer_start + READBUFFER*_1_MB, FR);
    uint32 offset = ((uint32)std::ceil( ( (float) READBUFFER*_1_MB) / ( (float) sizeof(HashCount) ) )) * sizeof(HashCount) ;
    this->hashCountManager = new ArrayHashCountManager(this->buffer_start + offset, this->buffer_end);
    
    this->total_read = 0;
    this->total_write = 0;
}

SplitHash::~SplitHash()
{
    delete this->FR;
    delete this->graph;
    delete this->hashCountManager;
}

void SplitHash::execute()
{
    while (this->graph->has_next()) {
        HashCount h = this->graph->next();
        this->hashCountManager->putSplitFiles(h);
    }

    std::string file_name = this->hashCountManager->getNewOutputFile();
    FileWriter FW = FileWriter(file_name);
    this->hashCountManager->writeToDisk(&FW);

    this->merge_files = this->hashCountManager->output_files;

    this->total_read += this->graph->total_read + this->hashCountManager->getTotalRead();
    this->total_write += this->graph->total_write + this->hashCountManager->getTotalWrite();
}
