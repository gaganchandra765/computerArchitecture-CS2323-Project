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
}