 //InvertedLinks.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include "include_types.h"
#include "FileReader.h"
#include "SplitHash.h"
#include "MergeHash.h"
#include "InvertedLinks.h"
#include "ArrayHashCountReader.h"
#include "Map.h"
#include "MapMerge.h"

#define BUFFERSIZE 500

void __cdecl _tmain(int argc, TCHAR *argv[])
{
    if (argc != 3)
    {
        printf("Usage Error: Incorrect number of arguments\n\n");
        _tprintf("Usage:\n\t%s <graph_file_name> <map_file_name>\n", argv[0]);
        return;
    }

    uint32 buffer_size = BUFFERSIZE * _1_MB;

    HashCount* buffer_start = new HashCount[buffer_size/sizeof(HashCount)];
    HashCount* buffer_end = buffer_start + buffer_size / sizeof(HashCount);
    clock_t begin, end; 

    printf("Starting split phase\n");
    std::deque<std::string> files;

    std::chrono::high_resolution_clock::time_point b1 = std::chrono::high_resolution_clock::now();
    {
        SplitHash splitHash(argv[1], buffer_start, buffer_end);
        splitHash.execute();
        files = splitHash.merge_files;
    }
    std::chrono::high_resolution_clock::time_point e1 = std::chrono::high_resolution_clock::now();
    printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e1 - b1).count());
    printf("Generated %d files\n", files.size());
    printf("Ending split phase\n");

    printf("Starting merge phase\n");
    std::chrono::high_resolution_clock::time_point b2 = std::chrono::high_resolution_clock::now();
    {
        MergeHash merge(buffer_start, buffer_end, files);
        merge.execute();
        files = merge.merge_files;
    }
    std::chrono::high_resolution_clock::time_point e2 = std::chrono::high_resolution_clock::now();
    printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e2 - b2).count());
    printf("Ending merge phase\n");

    printf("Staring map split phase\n");
    std::chrono::high_resolution_clock::time_point b3 = std::chrono::high_resolution_clock::now();
    {
        Map map(buffer_start, buffer_end, files[0], argv[2]);
        map.execute();
        files = map.output_files;
    }
    std::chrono::high_resolution_clock::time_point e3 = std::chrono::high_resolution_clock::now();
    printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e3 - b3).count());
    printf("Ending map split phase\n");

    printf("Starting map merge phase\n");
    std::chrono::high_resolution_clock::time_point b4 = std::chrono::high_resolution_clock::now();
    {
        MapMerge mapMerge(buffer_start, buffer_end, files);
        mapMerge.execute();
    }
    std::chrono::high_resolution_clock::time_point e4 = std::chrono::high_resolution_clock::now();
    printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e4 - b4).count());
    printf("Ending map merge phase\n");

    printf("DONE!\n");
    return;
}