// InvertedLinks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "file_manager.h"
#include <algorithm>
#include <string>
#include <vector>
#include <deque>

// #define _HAS_ITERATOR_DEBUGGING 0
#define BUFFERSIZE 500
#define _1_MB 1000000

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned short   ushort;

#pragma pack(push,1) // change struct packing to 1 byte
class HeaderGraph {
public:
    uint64 hash;
    uint32 len;
};

class CountHash {
public:
    uint64 hash;
    uint32 count;

    bool operator<(const CountHash& a) const
    {
        return hash < a.hash;
    }

    bool operator==(const CountHash& a) const
    {
        return hash == a.hash;
    }
};

class HeaderMap {
public:
    uint64 hash;
    ushort len;
};

class StringMap {
public:
    std::string name;
    uint32 count;
};

#pragma pack(pop) 

int main()
{
    return 0;
}

