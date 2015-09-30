 //InvertedLinks.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "include_types.h"
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include "FileReader.h""
#include "SplitHash.h"

#define BUFFERSIZE 500

void __cdecl _tmain(int argc, TCHAR *argv[])
{
    if (argc != 3)
    {
        printf("Usage Error: Incorrect number of arguments\n\n");
        _tprintf(TEXT("Usage:\n\t%s <text_file_name>\n"), argv[0]);
        return;
    }

    uint64 buffer_size = BUFFERSIZE * _1_MB;
    char * buffer = new char[buffer_size];

    SplitHash splitHash(argv[1], buffer, buffer + buffer_size);
    splitHash.execute();

    return;
}