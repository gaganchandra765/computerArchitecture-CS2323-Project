#ifndef ECC_UTILS_H
#define ECC_UTILS_H
#include<cstdint>
#include "ecc_metadata.h"

namespace ecc{
    uint64_t compute_ecc(uint32_t data);

    //  bits[31:0] = original value
    //  bits[38:32] = 7 bits ecc
    //  other bits = 0;
    
    uint64_t checkError(uint64_t encoded);

    uint64_t adaptive_check_error(uint64_t reg_val);
}

#endif
