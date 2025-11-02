#pragma once 

#include<cstdint>

namespace ecc{
    uint64_t compute_ecc(uint32_t data);

    //  bits[31:0] = original value
    //  bits[38:32] = 7 bits ecc
    //  other bits = 0;
}