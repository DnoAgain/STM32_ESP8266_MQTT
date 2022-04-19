#include "delay.h"

static uint8_t fac_us = 0;  
static uint32_t fac_ms = 0;

/**
  *@brief ��ʼ����ʱ����
  */
void delay_Init(void){
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTickƵ��Ϊ HCLK
  fac_us = SystemCoreClock/1000000;    //�����Ƿ�ʹ�� OS,fac_us����Ҫʹ��,SYSTICK ������ϵͳʱ�ӵ�Ƶ�� nMHz �е���ֵ n
  fac_ms = fac_us*1000;
}

/**
  *@brief ΢�뼶��ʱ
  *@parama num num�μ�������=num*(1/72000000)
  *@note ���233016us
  */
void delay_us(uint32_t num){
  SysTick->LOAD = num*fac_us;//1us�������� num*fac_us*1/72000000=num*1us
  SysTick->VAL = 0;//��ռ�����
  SysTick->CTRL |= (1<<0);//���ܼ�����
  while(!((SysTick->CTRL & 1<<16)));//�ж��Ƿ�������
  SysTick->VAL = 0;
  SysTick->CTRL &= ~(1<<0); //�رռ�����       
}

/**
  *@brief ���뼶��ʱ
  *@parama 
  *@note ���233ms,��num���233��2^24/72000��
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








