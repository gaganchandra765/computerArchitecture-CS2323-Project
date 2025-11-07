#ifndef ECC_METADATA_H
#define ECC_METADATA_H

#include <cstdint>

namespace ecc{
    // [0-31]  data
    // [32-38] ECC
    // [39]    not used yet
    // [40-41] ECC Mode
    // [42-43] Error History
    // [44-53] Frequency
    // [54-59] Sensitiviy
    // [60-63] Reserved

    constexpr uint64_t DATA_ECC_MASK = 0x7FFFFFFFFULL; //[0:38]
    constexpr uint64_t METADATA_MASK = ~DATA_ECC_MASK; // [63:39]

    constexpr int MODE_SHIFT = 40;
    constexpr uint64_t MODE_MASK = 0x3ULL;

    constexpr int HIST_SHIFT = 42;
    constexpr uint64_t HIST_MASK = 0x3ULL;

    constexpr int FREQ_SHIFT = 44;
    constexpr uint64_t FREQ_MASK = 0x3FFULL;

    constexpr int SENS_SHIFT =54;
    constexpr uint64_t SENS_MASK = 0x3FULL;


    enum ECCMode{
        MODE_NONE = 0, //00
        MODE_SEC = 1,//01;
        MODE_SECDED = 2 //10


    };

    inline uint8_t get_mode(uint64_t reg){ 
        return (reg >> MODE_SHIFT)&MODE_MASK;
    }
    inline uint8_t get_hist(uint64_t reg){
        return(reg>>HIST_SHIFT)&HIST_MASK;
    }
    inline uint16_t get_freq(uint64_t reg){ 
        return(reg>>FREQ_SHIFT)&FREQ_MASK;
    }
    inline uint8_t get_sens(uint64_t reg){ 
        return(reg>>SENS_SHIFT)&SENS_MASK;
    }

    inline uint64_t update_metadata(uint64_t reg, uint8_t mode, uint8_t hist, uint16_t freq, uint8_t sens){
        uint64_t clean_reg = reg&~((MODE_MASK<<MODE_SHIFT)|(HIST_MASK<<HIST_SHIFT)|(FREQ_MASK<<FREQ_SHIFT)|(SENS_MASK<<SENS_SHIFT));

        return clean_reg | (static_cast<uint64_t>(mode&MODE_MASK)<<MODE_SHIFT) | (static_cast<uint64_t>(hist&HIST_MASK)<<HIST_SHIFT) | (static_cast<uint64_t>(freq&FREQ_MASK)<<FREQ_SHIFT) | (static_cast<uint64_t>(sens&SENS_MASK)<<SENS_SHIFT);
    }



}


#endif