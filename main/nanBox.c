#include "ane.h"

double 
makeBox(uint32_t op, uint32_t tag) {
    return (union d64){.ubox = op | (int64_t)(tag) << 48}.nbox;
}

uint32_t
get_op(double nanbox) {
    return (uint32_t)(union d64){.nbox = nanbox}.ubox;
}
