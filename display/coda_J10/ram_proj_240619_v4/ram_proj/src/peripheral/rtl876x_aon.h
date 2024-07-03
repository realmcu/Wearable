#ifndef _RTL876X_AON_H_
#define _RTL876X_AON_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
uint16_t btaon_fast_read(uint16_t offset);
uint16_t btaon_fast_read_safe(uint16_t offset);
uint8_t btaon_fast_read_8b(uint16_t offset);
uint8_t btaon_fast_read_safe_8b(uint16_t offset);
void btaon_fast_write(uint16_t offset, uint16_t data);
void btaon_fast_write_safe(uint16_t offset, uint16_t data);
void btaon_fast_update(uint16_t offset, uint16_t mask, uint16_t data);
void btaon_fast_update_safe(uint16_t offset, uint16_t mask, uint16_t data);
void btaon_fast_write_8b(uint16_t offset, uint8_t data);
void btaon_fast_write_safe_8b(uint16_t offset, uint8_t data);
void btaon_fast_update_8b(uint16_t offset, uint8_t mask, uint8_t data);
void btaon_fast_update_safe_8b(uint16_t offset, uint8_t mask, uint8_t data);
#ifdef __cplusplus
}
#endif
#endif /* _RTL876X_AON_H_ */
