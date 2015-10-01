 //InvertedLinks.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include "include_types.h"
#include "FileReader.h"
#include "SplitHash.h"
#include "MergeHash.h"

#define BUFFERSIZE 500

void __cdecl _tmain(int argc, TCHAR *argv[])
{
    if (argc != 3)
    {
        printf("Usage Error: Incorrect number of arguments\n\n");
        _tprintf("Usage:\n\t%s <text_file_name>\n", argv[0]);
        return;
    }

    uint32 buffer_size = BUFFERSIZE * _1_MB;
    char * buffer = new char[buffer_size];

    SplitHash splitHash(argv[1], buffer, buffer + buffer_size);
    //splitHash.execute();
    for (int i = 1; i < 45; i++) {
        std::string str = OUTPUT_PREFIX + std::to_string(i) + OUTPUT_SUFFIX;
        splitHash.merge_files.push_back(str);
    }

    MergeHash merge(buffer, buffer + buffer_size, splitHash.merge_files);
    merge.execute();

    return;
}