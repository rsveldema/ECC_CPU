#pragma once


namespace ecc
{   
    static inline
    unsigned COUNT_BITS64(uint64_t v)
    {
        unsigned c = 0;
        for (unsigned i=0;i<64;i++)
        {
            if (v & (1UL << i))
            {
                c++;
            }        
        }

        return c;
    }
}