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

    printf("Starting split phase\n");
    SplitHash splitHash(argv[1], buffer_start, buffer_end);
    splitHash.execute();
    printf("Generated %d files\n", splitHash.merge_files.size());
    printf("Ending split phase\n");

    printf("Starting merge phase\n");
    MergeHash merge(buffer_start, buffer_end, splitHash.merge_files);
    merge.execute();
    printf("Ending merge phase\n");

    printf("Staring map split phase\n");
    Map map(buffer_start, buffer_end, merge.merge_files[0], argv[2]);
    map.execute();
    printf("Ending map split phase\n");

    printf("Starting map merge phase\n");
    MapMerge mapMerge(buffer_start, buffer_end, map.output_files);
    mapMerge.execute();
    printf("Ending map merge phase\n");

    printf("DONE!\n");
    return;
}