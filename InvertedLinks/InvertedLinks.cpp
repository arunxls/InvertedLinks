 //InvertedLinks.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <string>
#include <deque>
#include <chrono>
#include "include_types.h"
#include "FileReader.h"
#include "SplitHash.h"
#include "InvertedLinks.h"
#include "ArrayHashCountReader.h"
#include "Map.h"
#include "MapMergeManager.h"
#include "MergeHashManager.h"
#include <psapi.h>

#define BUFFERSIZE 496

//no except
void __cdecl _tmain(int argc, TCHAR *argv[]) noexcept
{
    if (argc != 3)
    {
        printf("Usage Error: Incorrect number of arguments\n\n");
        _tprintf("Usage:\n\t%s <graph_file_name> <map_file_name>\n", argv[0]);
        return;
    }

    uint64 total_read = 0;
    uint64 total_write = 0;

    uint32 buffer_size = BUFFERSIZE * _1_MB;

    HashCount* buffer_start = new HashCount[buffer_size/sizeof(HashCount)];
    HashCount* buffer_end = buffer_start + buffer_size / sizeof(HashCount);
    clock_t begin, end; 

    printf("Starting split phase\n");
    std::deque<std::string> files;

    std::chrono::high_resolution_clock::time_point b1 = std::chrono::high_resolution_clock::now();
    //{
    //    SplitHash splitHash(argv[1], buffer_start, buffer_end);
    //    splitHash.execute();
    //    files = splitHash.merge_files;
    //    printf("Total IO: read - %.2f GB; write - %.2f GB\n", (float)splitHash.total_read / _1_GB, (float)splitHash.total_write / _1_GB);
    //    total_read += (float)splitHash.total_read;
    //    total_write += (float)splitHash.total_write;
    //}
    //std::chrono::high_resolution_clock::time_point e1 = std::chrono::high_resolution_clock::now();
    //printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e1 - b1).count());
    //printf("Generated %d files\n", files.size());
    //printf("Ending split phase\n");

    //printf("\n");

    ////for (int i = 1; i < 48; i++) {
    ////    std::string temp = "split" + std::to_string(i);
    ////    files.push_back(temp);
    ////}

    //printf("Starting merge phase\n");
    //std::chrono::high_resolution_clock::time_point b2 = std::chrono::high_resolution_clock::now();
    //{
    //    MergeHashManager merge(buffer_start, buffer_end, files);
    //    merge.execute();
    //    files = merge.merge_files;
    //    printf("Total IO: read - %.2f GB; write - %.2f GB\n", (float)merge.total_read / _1_GB, (float)merge.total_write / _1_GB);
    //    total_read += (float)merge.total_read;
    //    total_write += (float)merge.total_write;
    //}
    //std::chrono::high_resolution_clock::time_point e2 = std::chrono::high_resolution_clock::now();
    //printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e2 - b2).count());
    //printf("Ending merge phase\n");

    //printf("\n");

    //printf("Staring map split phase\n");
    //std::chrono::high_resolution_clock::time_point b3 = std::chrono::high_resolution_clock::now();
    //{
    //    Map map(buffer_start, buffer_end, files[0], argv[2]);
    //    map.execute();
    //    files = map.output_files;
    //    printf("Total IO: read - %.2f GB; write - %.2f GB\n", (float)map.total_read / _1_GB, (float)map.total_write / _1_GB);
    //    total_read += (float)map.total_read;
    //    total_write += (float)map.total_write;
    //}
    //std::chrono::high_resolution_clock::time_point e3 = std::chrono::high_resolution_clock::now();
    //printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e3 - b3).count());
    //printf("Ending map split phase\n");

    //printf("\n");
    
    for (int i = 1; i < 13; i++) {
        std::string temp = "string" + std::to_string(i);
        files.push_back(temp);
    }

    printf("Starting map merge phase\n");
    std::chrono::high_resolution_clock::time_point b4 = std::chrono::high_resolution_clock::now();
    {
        MapMergeManager mapMerge(buffer_start, buffer_end, files);
        mapMerge.execute();
        printf("Total IO: read - %.2f GB; write - %.2f GB\n", (float)mapMerge.total_read / _1_GB, (float)mapMerge.total_write / _1_GB);
        total_read += (float)mapMerge.total_read;
        total_write += (float)mapMerge.total_write;
    }
    std::chrono::high_resolution_clock::time_point e4 = std::chrono::high_resolution_clock::now();
    printf("Took %lld seconds\n", std::chrono::duration_cast<std::chrono::seconds>(e4 - b4).count());
    printf("Ending map merge phase\n");

    printf("\n");
    
    printf("Overall stats - RunTime: %lld seconds; Total read %.2f GB; Total write %.2f GB\n",
        std::chrono::duration_cast<std::chrono::seconds>(e4 - b1).count(),
        (float)total_read/_1_GB, (float)total_write/_1_GB);

    {
        PROCESS_MEMORY_COUNTERS pp;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, GetCurrentProcessId());
        GetProcessMemoryInfo(hProcess, &pp, sizeof(PROCESS_MEMORY_COUNTERS));
        printf("Peaking working set : %.2f MB\n", (double)pp.PeakWorkingSetSize / _1_MB);
    }

    printf("DONE!\n");
    return;
}