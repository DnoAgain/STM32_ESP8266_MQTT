#include "delay.h"

static uint8_t fac_us = 0;  
static uint32_t fac_ms = 0;

/**
  *@brief 初始化延时函数
  */
void delay_Init(void){
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为 HCLK
  fac_us = SystemCoreClock/1000000;    //不论是否使用 OS,fac_us都需要使用,SYSTICK 参数是系统时钟的频率 nMHz 中的数值 n
  fac_ms = fac_us*1000;
}

/**
  *@brief 微秒级延时
  *@parama num num次计数周期=num*(1/72000000)
  *@note 最大233016us
  */
void delay_us(uint32_t num){
  SysTick->LOAD = num*fac_us;//1us计数次数 num*fac_us*1/72000000=num*1us
  SysTick->VAL = 0;//清空计数器
  SysTick->CTRL |= (1<<0);//是能计数器
  while(!((SysTick->CTRL & 1<<16)));//判断是否计数完成
  SysTick->VAL = 0;
  SysTick->CTRL &= ~(1<<0); //关闭计数器       
}

/**
  *@brief 毫秒级延时
  *@parama 
  *@note 最大233ms,即num最大233（2^24/72000）
  */
void delay_ms(uint16_t num){
  uint32_t temp;
  SysTick->LOAD = num*fac_ms;
  SysTick->VAL = 0x00;
  SysTick->CTRL|=(1<<0);
  do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
  SysTick->CTRL&=~(1<<0);
  SysTick->VAL = 0x00;
}








