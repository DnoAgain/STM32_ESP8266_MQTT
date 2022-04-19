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
  
  SCCB_SDA_OUT(); //��������Ϊ��λ
}

/**************************************************************************************
 * ��  �� : ����OV7670оƬSCCB�ӿ���ʼ�ź�(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : ��
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
 * ��  �� : ����OV7670оƬSCCB�ӿ�ֹͣ�ź�(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : ��
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
 * ��  �� : ����OV7670оƬSCCB�ӿڲ���NAK�ź�(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : ��
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
 * ��  �� : OV7670оƬSCCB�ӿ�дһ���ֽ�����(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : 0,�ɹ�;1,ʧ��. 
 **************************************************************************************/
uint8_t SCCB_WriteAbyte(uint8_t dat)
{
	uint8_t i,temp;	 
	for(i=0;i<8;i++)     //ѭ��8�η���һ���ֽ�����
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
	SCCB_SDA_IN();		  //����SDAΪ���� 
	delay_us(50);
	SCCB_SCL_H;			    //���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(50);
	if(SCCB_READ_SDA)
		temp=1;           //SDA�ź�Ϊ�ߵ�ƽ��ʾ����ʧ�ܣ�����1
	else 
		temp=0;           //SDA�ź�Ϊ�͵�ƽ��ʾ���ͳɹ�������0
	SCCB_SCL_L;		 
	SCCB_SDA_OUT();		  //��ȡSDA�źź�������SDAΪ���    
	return temp;  
}	 

/**************************************************************************************
 * ��  �� : OV7670оƬSCCB�ӿڶ�һ���ֽ�����(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : ������һ���ֽ�����
 **************************************************************************************/
uint8_t SCCB_ReadAbyte(void)
{
	uint8_t i,temp;    
	temp=0;             //����ʱ������ʼ��ֵΪ0
	SCCB_SDA_IN();		  //����SDAΪ����  
	for(i=8;i>0;i--) 	  //ѭ��8�ν�������
	{		     	  
		delay_us(50);
		SCCB_SCL_H;
		temp=temp<<1;
		if(SCCB_READ_SDA)
			temp++;   
		delay_us(50);
		SCCB_SCL_L;
	}	
	SCCB_SDA_OUT();		 //��ȡSDA�źź�������SDAΪ���     
	return temp;
} 							    

/**************************************************************************************
 * ��  �� : OV7670оƬSCCB�ӿ�д�Ĵ�������(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : 0,�ɹ�;1,ʧ��.
 **************************************************************************************/
uint8_t SCCB_Write_Reg(uint8_t reg,uint8_t data)
{
	uint8_t temp=0;
	SCCB_Start(); 					            //����SCCB����
	if(SCCB_WriteAbyte(SCCB_ID))
		temp=1;	                          //д����ID	  
	delay_us(100);
  if(SCCB_WriteAbyte(reg))
		temp=1;		                        //д�Ĵ�����ַ	  
	delay_us(100);
  if(SCCB_WriteAbyte(data))
		temp=1; 	                        //д����	 
  SCCB_Stop();	                      //����SCCB����
  return	temp;
}		  					    

/**************************************************************************************
 * ��  �� : OV7670оƬSCCB�ӿڶ��Ĵ�������(ģ��SCCB)
 * ��  �� : ��
 * ����ֵ : �����ļĴ���ֵ
 **************************************************************************************/
uint8_t SCCB_Read_Reg(uint8_t reg)
{
	uint8_t temp=0;
	SCCB_Start(); 				             //����SCCB����
	SCCB_WriteAbyte(SCCB_ID);		       //д����ID	  
  delay_us(100);	 
  SCCB_WriteAbyte(reg);		           //д�Ĵ�����ַ	  
	delay_us(100);	  
	SCCB_Stop();   	                   //����SCCB����
	delay_us(100);	   
	SCCB_Start(); 				             //����SCCB����
	SCCB_WriteAbyte(SCCB_ID|0X01);   	 //���Ͷ�����	  
	delay_us(100);
  temp=SCCB_ReadAbyte();		         //��ȡ����
  SCCB_No_Ack();                     //����NAK�ź� 
  SCCB_Stop();  	                   //����SCCB����
  return temp;
}





