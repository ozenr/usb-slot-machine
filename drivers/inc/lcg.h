#ifndef LCG_H
#define LCG_H

// External Libraries
#include <stdint.h>

// Define LCG Parameters
#define A 16807
#define M 2147483647

/*
 * @brief Generates pseudo-random integer
 */
uint32_t randint(uint32_t seed);

/*
 * @brief Takes pseudo-random generated integer and squeezes to produce 8 bit
 * integer
 */
uint8_t squeeze(uint32_t randnum, uint8_t squeeze_factor);

#endif