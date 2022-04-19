#include "sccb.h"


void MX_SCCB_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /*Configure GPIO pin : PB10 SCCB_D*/
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB11 SCCB_C*/
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  SCCB_SDA_OUT(); //数据线先为高位
}

/**************************************************************************************
 * 描  述 : 配置OV7670芯片SCCB接口起始信号(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SCCB_Start(void)
{
    SCCB_SDA_H;     
    SCCB_SCL_H;	    
    delay_us(50);  
    SCCB_SDA_L;
    delay_us(50);	 
    SCCB_SCL_L;	   
}

/**************************************************************************************
 * 描  述 : 配置OV7670芯片SCCB接口停止信号(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SCCB_Stop(void){
    SCCB_SDA_L;
    delay_us(50);	 
    SCCB_SCL_H;	
    delay_us(50); 
    SCCB_SDA_H;	
    delay_us(50);
} 

/**************************************************************************************
 * 描  述 : 配置OV7670芯片SCCB接口产生NAK信号(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SCCB_No_Ack(void)
{
	delay_us(50);
	SCCB_SDA_H;	
	SCCB_SCL_H;	
	delay_us(50);
	SCCB_SCL_L;	
	delay_us(50);
	SCCB_SDA_L;	
	delay_us(50);
}

/**************************************************************************************
 * 描  述 : OV7670芯片SCCB接口写一个字节数据(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 0,成功;1,失败. 
 **************************************************************************************/
uint8_t SCCB_WriteAbyte(uint8_t dat)
{
	uint8_t i,temp;	 
	for(i=0;i<8;i++)     //循环8次发送一个字节数据
	{
		if(dat&0x80)
			SCCB_SDA_H;	
		else 
			SCCB_SDA_L;
		dat<<=1;
		delay_us(50);
		SCCB_SCL_H;	
		delay_us(50);
		SCCB_SCL_L;		   
	}			 
	SCCB_SDA_IN();		  //设置SDA为输入 
	delay_us(50);
	SCCB_SCL_H;			    //接收第九位,以判断是否发送成功
	delay_us(50);
	if(SCCB_READ_SDA)
		temp=1;           //SDA信号为高电平表示发送失败，返回1
	else 
		temp=0;           //SDA信号为低电平表示发送成功，返回0
	SCCB_SCL_L;		 
	SCCB_SDA_OUT();		  //读取SDA信号后，需设置SDA为输出    
	return temp;  
}	 

/**************************************************************************************
 * 描  述 : OV7670芯片SCCB接口读一个字节数据(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 读到的一个字节数据
 **************************************************************************************/
uint8_t SCCB_ReadAbyte(void)
{
	uint8_t i,temp;    
	temp=0;             //对临时变量初始化值为0
	SCCB_SDA_IN();		  //设置SDA为输入  
	for(i=8;i>0;i--) 	  //循环8次接收数据
	{		     	  
		delay_us(50);
		SCCB_SCL_H;
		temp=temp<<1;
		if(SCCB_READ_SDA)
			temp++;   
		delay_us(50);
		SCCB_SCL_L;
	}	
	SCCB_SDA_OUT();		 //读取SDA信号后，需设置SDA为输出     
	return temp;
} 							    

/**************************************************************************************
 * 描  述 : OV7670芯片SCCB接口写寄存器操作(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 0,成功;1,失败.
 **************************************************************************************/
uint8_t SCCB_Write_Reg(uint8_t reg,uint8_t data)
{
	uint8_t temp=0;
	SCCB_Start(); 					            //启动SCCB传输
	if(SCCB_WriteAbyte(SCCB_ID))
		temp=1;	                          //写器件ID	  
	delay_us(100);
  if(SCCB_WriteAbyte(reg))
		temp=1;		                        //写寄存器地址	  
	delay_us(100);
  if(SCCB_WriteAbyte(data))
		temp=1; 	                        //写数据	 
  SCCB_Stop();	                      //结束SCCB传输
  return	temp;
}		  					    

/**************************************************************************************
 * 描  述 : OV7670芯片SCCB接口读寄存器操作(模拟SCCB)
 * 入  参 : 无
 * 返回值 : 读到的寄存器值
 **************************************************************************************/
uint8_t SCCB_Read_Reg(uint8_t reg)
{
	uint8_t temp=0;
	SCCB_Start(); 				             //启动SCCB传输
	SCCB_WriteAbyte(SCCB_ID);		       //写器件ID	  
  delay_us(100);	 
  SCCB_WriteAbyte(reg);		           //写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   	                   //结束SCCB传输
	delay_us(100);	   
	SCCB_Start(); 				             //启动SCCB传输
	SCCB_WriteAbyte(SCCB_ID|0X01);   	 //发送读命令	  
	delay_us(100);
  temp=SCCB_ReadAbyte();		         //读取数据
  SCCB_No_Ack();                     //发送NAK信号 
  SCCB_Stop();  	                   //结束SCCB传输
  return temp;
}





