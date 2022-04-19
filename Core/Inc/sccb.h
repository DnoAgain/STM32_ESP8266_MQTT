#ifndef __SCCB_H__
#define __SCCB_H__

#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "delay.h"
#include "sys.h"
/* USER CODE BEGIN Private defines */

  
#define SCCB_SDA_IN()         {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=0X0000800;}
#define SCCB_SDA_OUT()        {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=0X0000300;}

#define SCCB_SDA        PBout(10)      
#define SCCB_SCL        PBin(11)

#define SCCB_READ_SDA   PBin(10) 
#define SCCB_ID         0x42

#define SCCB_SDA_L      PBout(10)=0
#define SCCB_SDA_H      PBout(10)=1 

#define SCCB_SCL_L      PBout(11)=0
#define SCCB_SCL_H      PBout(11)=1


/* USER CODE END Private defines */
  
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
  
void MX_SCCB_Init(void);
void SCCB_Start(void); 
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WriteAbyte(uint8_t dat);
uint8_t SCCB_ReadAbyte(void);
uint8_t SCCB_Write_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_Read_Reg(uint8_t reg);

  
  
  
#ifdef __cplusplus
}
#endif
#endif

