#include "ecc/ecc_utils.h"

#include <cstring>

namespace ecc{
    uint64_t compute_ecc(uint32_t data){
        uint8_t p[6]={0};
        uint8_t p_all = 0;

        for(int i=0;i<32;i++){
            uint8_t data_bit = (data>>i)&1;

            uint32_t pos = i+1;

            if((pos&1)!=0){
                p[0]^=data_bit;

            }

            if((pos&2)!=0){
                p[1]^=data_bit;

            }
            if((pos&4)!=0){
                p[2]^=data_bit;

            }
            if((pos&8)!=0){
                p[3]^=data_bit;

            }
            if((pos&16)!=0){
                p[4]^=data_bit;

            }

            if((pos&32)!=0){
                p[5]^=data_bit;

            }


        }

        uint32_t hamming_code = (p[5]<<5) | (p[4]<<4) | (p[3]<<3) | (p[2]<<2) | (p[1]<<1) | p[0];

        uint8_t data_parity = __builtin_parityl(data); // returns 1 for odd no.of set bits, 0 for even

        uint8_t hamming_parity = __builtin_parity(hamming_code & 0x3F); // only 6 bits
        p_all = data_parity ^hamming_parity; // overall parity bit
        uint32_t final_ecc = (p_all<<6)|hamming_code;

        uint64_t result = (static_cast<uint64_t>(final_ecc)<<32)|static_cast<uint64_t>(data);

        return result;


    }
    
    uint64_t checkError(uint64_t encoded) {
    // Extract data and received ECC
    uint32_t data = static_cast<uint32_t>(encoded & 0xFFFFFFFFULL);
    uint32_t ecc_received = static_cast<uint32_t>((encoded >> 32) & 0x7F); // 7 bits

    uint8_t p_received[6];
    uint8_t p_all_received = (ecc_received >> 6) & 1;
    for (int i = 0; i < 6; ++i) {
        p_received[i] = (ecc_received >> i) & 1;
    }

    // Recompute parity bits from received data
    uint8_t p_computed[6] = {0};
    for (int i = 0; i < 32; ++i) {
        uint8_t bit = (data >> i) & 1;
        uint32_t pos = i + 1;
        if (pos & 1)  p_computed[0] ^= bit;
        if (pos & 2)  p_computed[1] ^= bit;
        if (pos & 4)  p_computed[2] ^= bit;
        if (pos & 8)  p_computed[3] ^= bit;
        if (pos & 16) p_computed[4] ^= bit;
        if (pos & 32) p_computed[5] ^= bit;
    }

    // Syndrome
    uint8_t syndrome = 0;
    for (int i = 0; i < 6; ++i) {
        if (p_computed[i] != p_received[i]) {
            syndrome |= (1 << i);
        }
    }

    // Overall parity
    uint8_t data_parity = __builtin_parityl(data);
    uint8_t hamming_parity = __builtin_parity(ecc_received & 0x3F);
    uint8_t p_all_computed = data_parity ^ hamming_parity;
    bool parity_mismatch = (p_all_computed != p_all_received);

    bool corrected = false;

    // SINGLE BIT ERROR IN DATA?
    if (syndrome != 0 && parity_mismatch) {
        int error_pos = syndrome;
        if (error_pos >= 1 && error_pos <= 32) {
            data ^= (1U << (error_pos - 1));
            corrected = true;
        }
    }

    // RECOMPUTE FRESH ECC
    uint64_t fresh_ecc = ecc::compute_ecc(data);

    // FIXED: Parentheses around (fresh_ecc & 0x7F)
    uint64_t result = ((fresh_ecc & 0x7FULL) << 32) | static_cast<uint64_t>(data);

    return result;
}

}

