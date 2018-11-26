#include "cache.h"
#include "bits.h"

int get_set (Cache *cache, address_type address) {
  /* TODO:
     Extract the set bits from a 32-bit address.
   */

int set = cache->set_bits + cache->block_bits;
int extract = 32 - set;

  return address << extract >> (cache->block_bits + extract);
}

int get_line (Cache *cache, address_type address) {
  /* TODO:
     Extract the tag bits from a 32-bit address.
   */

  return address >> (cache->set_bits + cache->block_bits);
}

int get_byte (Cache *cache, address_type address) {
  /* TODO
     Extract the block (byte index) bits from a 32-bit address.
   */
  
    int byte = cache->set_bits + cache->block_bits;
    int extract = 32 - byte;
    int block = extract + cache->set_bits;

  return address << block >> block;
}
