#pragma once

#include <string>

// #define _HAS_ITERATOR_DEBUGGING 0
#define _1_MB 1000000

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned short   ushort;

const std::string OUTPUT_PREFIX = "__merge__";
const std::string OUTPUT_SUFFIX = ".dat";

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

#pragma pack(pop)