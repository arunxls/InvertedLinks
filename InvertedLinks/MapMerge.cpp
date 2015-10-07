#include "stdafx.h"
#include "MapMerge.h"
#include "FileReader.h"
#include "FileWriter.h"

MapMerge::MapMerge()
{
}

MapMerge::MapMerge(void *start, void *end, std::deque<std::string> merge_files)
{
    this->start = (char*)start;
    this->end = (char*)end;
    this->start_offset = this->start;
    this->end_offset = this->start;

    this->merge_files = merge_files;

    uint32 offset = (this->end - this->start) / 4;
    
    this->merge_1 = new ArrayStringCount(this->start, this->start + offset);
    this->merge_2 = new ArrayStringCount(this->start + offset, this->start + 2*offset);
    this->output = new ArrayStringCount(this->start + 2 * offset, this->end);

    this->file_count = 1;
}


MapMerge::~MapMerge()
{
    delete this->merge_1;
    delete this->merge_2;
    delete this->output;
}

void MapMerge::execute()
{
    while (this->merge_files.size() > 1) {
        std::string str1 = this->merge_files.front();
        this->merge_files.pop_front();
        std::string str2 = this->merge_files.front();
        this->merge_files.pop_front();

        {
            FileReader FR1(str1);
            this->merge_1->setFileReader(&FR1);

            FileReader FR2(str2);
            this->merge_2->setFileReader(&FR2);

            std::string str3;
            bool final_run = false;
            if (this->merge_files.size() != 0) {
                str3 = this->getNewOutputFile();
            }
            else {
                str3 = "output.txt";
                final_run = true;
            }

            FileWriter FW(str3);
            this->output->setFileWriter(&FW);

            while (this->merge_1->has_next() || this->merge_2->has_next()) {
                StringCount* h;
                if (this->merge_1->has_next() && this->merge_2->has_next()) {
                    StringCount* h1 = this->merge_1->current();
                    StringCount* h2 = this->merge_2->current();

                    h = *h1 > *h2 ? this->merge_1->next() : this->merge_2->next();
                }
                else if (this->merge_1->has_next()) {
                    h = this->merge_1->next();
                }
                else {
                    h = this->merge_2->next();
                }

                if (final_run) {
                    this->output->putFinalRun(h);
                }
                else {
                    this->output->putSingleFile(h);
                }
            }

            this->output->writeToDisk(this->output->FW);
            this->merge_files.push_back(str3);

            this->total_read = this->merge_1->total_read + this->merge_2->total_read + this->output->total_read;
            this->total_write = this->merge_1->total_write + this->merge_2->total_write + this->output->total_write;
        }
        DeleteFile(TEXT(str1.c_str()));
        DeleteFile(TEXT(str2.c_str()));
    }

    return;
}

std::string MapMerge::getNewOutputFile()
{
    return "final" + std::to_string(file_count++);
}
