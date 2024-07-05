#ifndef _TIME_ADAPTER_H_
#define _TIME_ADAPTER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_timer(void);

void delay_us(uint32_t us);

void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _TIME_ADAPTER_H_ */
