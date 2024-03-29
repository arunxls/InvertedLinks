#pragma once

#include <string>
#include <windows.h>
#define _HAS_ITERATOR_DEBUGGING 0
#define _1_MB 1000000
#define _1_GB 1000000000
#define DEBUG 0

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned short   ushort;

const std::string OUTPUT_PREFIX = "split";
const std::string OUTPUT_SUFFIX = "";
const HANDLE gHashWriteSemaphone = CreateSemaphore(NULL, 1, 1, NULL);;

#pragma pack(push,1) // change struct packing to 1 byte
class HeaderGraph {
public:
    uint64 hash;
    uint32 len;
};

class HashCount {
public:
    uint64 hash;
    uint32 count;

    bool operator<(const HashCount& a) const
    {
        return hash < a.hash;
    }

    bool operator>(const HashCount& a) const
    {
        return hash > a.hash;
    }

    bool operator==(const HashCount& a) const
    {
        return hash == a.hash;
    }

    HashCount& operator+=(const HashCount& rhs)
    {
        count += rhs.count;
        return *this;
    }

    HashCount& operator=(const HashCount& rhs)
    {
        hash = rhs.hash;
        count = rhs.count;
        return *this;
    }
};

class HeaderMap {
public:
    uint64 hash;
    ushort len;
};

class StringCount {
public:
    uint32 count;
    ushort len;

    bool operator<(const StringCount& a) const
    {
        return count < a.count;
    }

    bool operator>(const StringCount& a) const
    {
        return count > a.count;
    }
};
#pragma pack(pop)