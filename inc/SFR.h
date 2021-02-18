#ifndef SFR_DEFS_H
#define SFR_DEFS_H
#include <stdint.h>
#include <stdbool.h>

#define SFR_IO8(addr) (*(volatile uint8_t *)(addr))
#define SFR_IO16(addr) (*(volatile uint16_t *)(addr))
#define SFR_IO32(addr) (*(volatile uint32_t *)(addr))
#define _BV(bit) (1 << (bit))
#define bit_is_set(word, bit)           ((word) & _BV(bit))
#define bit_is_clear(word,bit)          (!((word) & _BV(bit)))

#endif // SFR_DEFS_H



