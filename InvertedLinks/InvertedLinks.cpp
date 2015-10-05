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

    //SplitHash splitHash(argv[1], buffer_start, buffer_end);
    //splitHash.execute();
    //for (int i = 93; i < 94; i++) {
    //    std::string str = OUTPUT_PREFIX + std::to_string(i) + OUTPUT_SUFFIX;
    //    splitHash.merge_files.push_back(str);
    //}

    //int count = 0;
    //for (std::string s : splitHash.merge_files) {
    //    ArrayHashCountReader foo = ArrayHashCountReader(buffer_start, buffer_end);
    //    FileReader fr = FileReader(s);
    //    foo.setFileReader(&fr);
    //    while (foo.has_next()) {
    //        HashCount h = foo.next();
    //        if (h.hash == 17082586730649405658ULL) {
    //            printf("%s = %d\n",s, h.count);
    //            count += h.count;
    //        }
    //    }
    //}

    //printf("%d", count);

    //MergeHash merge(buffer_start, buffer_end, splitHash.merge_files);
    //merge.execute();

    Map map(buffer_start, buffer_end, "merge93", argv[2]);
    map.execute();

    return;
}