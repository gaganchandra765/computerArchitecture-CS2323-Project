#include "ecc/ecc_utils.h"

#include <cstring>
#include <iostream>

namespace ecc{
    uint64_t compute_ecc(uint32_t data){
        // std::cout << data << "\n";
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
    // extract data and intial ECC
    uint32_t data = static_cast<uint32_t>(encoded & 0xFFFFFFFFULL);
    uint32_t ecc_received = static_cast<uint32_t>((encoded >> 32) & 0x7F); // 7 bits

    uint8_t p_received[6];
    uint8_t p_all_received = (ecc_received >> 6) &1;
    for (int i = 0; i < 6; ++i){
        p_received[i] = (ecc_received >> i) & 1;
    }

    // compute again parity bits from received data
    uint8_t p_computed[6] = {0};
    for (int i = 0; i < 32; ++i){
        uint8_t bit = (data>> i)& 1;
        uint32_t pos = i +1;
        if (pos & 1) p_computed[0] ^= bit;
        if (pos & 2) p_computed[1] ^= bit;
        if (pos & 4) p_computed[2] ^= bit;
        if (pos & 8)  p_computed[3] ^= bit;
        if (pos & 16) p_computed[4] ^= bit;
        if (pos & 32) p_computed[5] ^= bit;
    }

    uint8_t syndrome = 0;
    for (int i = 0; i < 6; ++i){
        if (p_computed[i] != p_received[i]) {
            syndrome |= (1 << i);
    }
    }

    // overall parity
    uint8_t data_parity = __builtin_parityl(data);
    uint8_t hamming_parity = __builtin_parity(ecc_received & 0x3F);
    uint8_t p_all_computed = data_parity ^ hamming_parity;
    bool parity_mismatch = (p_all_computed != p_all_received);

    // bool corrected = false;

    // no errors
    if(syndrome==0&&!parity_mismatch){
        return encoded;

    }

    if(syndrome==0&&parity_mismatch){
        uint64_t new_ecc = ecc::compute_ecc(data);
        return new_ecc;
    }
    // if single error 
    if (syndrome != 0 && parity_mismatch){
        int error_pos = syndrome;
        if (error_pos >= 1 && error_pos <= 32){
            data ^=(1U<<(error_pos - 1));
            // corrected = true;
    }
        uint64_t newData = compute_ecc(data);
        return newData;
    }
    else{
        // more than 1 error, can not correct it 
    }

    return encoded;
}
    uint64_t adaptive_check_error(uint64_t reg_val){
        // uint64_t metadata_bits = reg_val & METADATA_MASK;
        uint8_t mode = get_mode(reg_val);
        uint8_t hist =get_hist(reg_val);
        uint16_t freq = get_freq(reg_val);
        uint8_t sens = get_sens(reg_val);


        if(freq<1023){
            freq+=1;
        }

        uint64_t processed_val = reg_val;

        if(mode!=MODE_NONE){
            processed_val = checkError(reg_val);

            if((processed_val&DATA_ECC_MASK)!=(reg_val&DATA_ECC_MASK)){
                if(hist<3){
                    hist+=1;
                }
            }

        }

        bool high_usage =(freq>700);
        bool high_sensitivity = (sens>=4);
        bool recent_errors =(hist>=1);

        if(high_usage||high_sensitivity||recent_errors){
            mode = MODE_SECDED;
        }
        else if(freq>300){
            mode = MODE_SEC;
        }
        else{
            mode = MODE_SEC;
        }

        return update_metadata(processed_val,mode,hist,freq,sens);


    }

}

