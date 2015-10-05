#include "stdafx.h"
#include "MergeHash.h"
#include <algorithm>

MergeHash::MergeHash(void *start, void *end, std::deque<std::string> merge_files)
{
    this->start = (char*) start;
    this->end = (char*) end;
    this->merge_files = merge_files;
    
    uint32 read_buffer = ((MERGESIZE * _1_MB) / sizeof(HashCount))*sizeof(HashCount);
    this->read_1 = ArrayHashCountReader(this->start, this->start + read_buffer);
    this->read_2 = ArrayHashCountReader(this->start + read_buffer + 1, (this->start + read_buffer + 1) + read_buffer);
    this->write_merged = ArrayHashCountReader((((this->start + read_buffer + 1) + read_buffer) + 1), end);

    this->file_count = merge_files.size() + 1;
}

MergeHash::~MergeHash()
{
}

void MergeHash::execute()
{
    while (this->merge_files.size() > 1) {
        std::string str1 = this->merge_files.front();
        FileReader FR1(str1);
        this->merge_files.pop_front();
        this->read_1.setFileReader(&FR1);

        std::string str2 = this->merge_files.front();
        FileReader FR2(str2);
        this->merge_files.pop_front();
        this->read_2.setFileReader(&FR2);

        std::string str3 = this->getNewOutputFile();
        FileWriter FW(str3);
        this->write_merged.setFileWriter(&FW);

        while (this->read_1.has_next() || this->read_2.has_next()) {
            HashCount h;
            if (this->read_1.has_next() && this->read_2.has_next()) {
                HashCount h1 = this->read_1.current();
                HashCount h2 = this->read_2.current();

                h = h1 < h2 ? this->read_1.next() : this->read_2.next();
            }
            else if (this->read_1.has_next()) {
                h = this->read_1.next();
            }
            else {
                h = this->read_2.next();
            }
            
            this->write_merged.putSingleFile(h);
        }

        this->write_merged.compact();
        this->write_merged.writeToDisk(str3);
        this->merge_files.push_back(str3);
        
        DeleteFile(TEXT(str1.c_str()));
        DeleteFile(TEXT(str2.c_str()));
    }

    return;
}

std::string MergeHash::getNewOutputFile()
{
    return OUTPUT_PREFIX + std::to_string(this->file_count++) + OUTPUT_SUFFIX;
}