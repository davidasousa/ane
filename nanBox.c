#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

// double: 64 bits 3141.592654 = 3.141592654 * 10^3
// sign bit | 11 bits exponents | 3 bits | 49 bit
//            11 bits are to 1  | tag    | value

enum tags // Tags Are The Class Of The Data Type - "Tagged"
{
    tag1 = 0x7ff8,
    tag2,
    tag3,
    tag4,
};

// Both The Unsigned 64 Bit Int And The Double Exist In The Same Place In Memory
// The U64 is meant for accessing the tag and the value
// The D64 is the nanBox itself

typedef union data64 {
    uint64_t u64;
    double d64;
} data64;

typedef double nanBox;
typedef uint32_t val32;

nanBox makeBox(val32 tag, val32 val)
{
    data64 box;
    box.u64 = ((uint64_t)tag << 48 | val);
    nanBox nanBox = box.d64;
    return nanBox;
}

int getTag(nanBox box)
{
    data64 d64 = (data64)box;
    int tag = d64.u64 >> 48;
    return tag;
}

int getVal(nanBox box)
{
    int val = (val32)box;
    return val;
}

int main(void)
{
    nanBox box = makeBox(tag2, 4987);
    
    int tag = getTag(box);
    int val = getVal(box);

    printf("%d %d\n", tag, box);

    return EXIT_SUCCESS;
}
