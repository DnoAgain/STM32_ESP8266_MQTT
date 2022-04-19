#ifndef __DELAY_H__
#define __DELAY_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h" 
  
void delay_Init(void);   
void delay_us(uint32_t num);  
void delay_ms(uint16_t num);
  
  
    
#ifdef __cplusplus
}
#endif
#endif

