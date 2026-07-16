#include "lcg.h"

uint32_t randint(uint32_t seed) { return (A * seed) % M; }

uint8_t squeeze(uint32_t randnum, uint8_t squeeze_factor) {
  return (uint8_t)randnum % squeeze_factor;
}
