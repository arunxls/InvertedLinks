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
#include "InvertedLinks.h"
#include "ArrayHashCountReader.h"

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

    HashCount* buffer_start = new HashCount[buffer_size/sizeof(HashCount)];
    HashCount* buffer_end = buffer_start + buffer_size / sizeof(HashCount);

    SplitHash splitHash(argv[1], buffer_start, buffer_end);
    splitHash.execute();
    //for (int i = 1; i < 48; i++) {
    //    std::string str = OUTPUT_PREFIX + std::to_string(i) + OUTPUT_SUFFIX;
    //    splitHash.merge_files.push_back(str);
    //}

    MergeHash merge(buffer_start, buffer_end, splitHash.merge_files);
    merge.execute();

    return;
}