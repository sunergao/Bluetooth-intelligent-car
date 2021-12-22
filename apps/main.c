#include "MOTOR.h"
#include "stm32f10x.h"         //�ٷ���
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include <stdio.h>


/*

#ifdef->endif�����ݲ��ֺ�PUTCHAR_PROTOTYPE{}����������C���Ե��﷨�������룬
����ͷ����������#include<stdio.h>,�����ӵĻ�C���Ե��﷨�Ϳ�����������

*/


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//�жϷ�����  
u8 i;
u8 flag;  //����һ����־λ
void USART1_IRQHandler(void)  
  
{  
     if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)      //���ָ���� USART1 �жϷ������
      { 
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //��� USART1 ���жϴ�����λ
			////GPIO_ResetBits(GPIOC,GPIO_Pin_6);                 //����LED����Ϊ�жϷ�������ָʾ��
			i=  USART_ReceiveData(USART1);                    //���� USART1 ������յ�������
		//if�������ݲ��ֿ��Ա�֤�������յ���flag��������ȷ�ģ�������ӵĻ��ᵼ�½��ղ�����ȷ������
		  if(i=='0')
				{                 
					 flag=0;
				}
				
			if(i=='1')
				{
					 flag=1;
				}
				
			if(i=='2')
			   {
					 flag=2;
				}
			   
			if(i=='3')
			   {
								 
					flag=3;
			   }
			   
			if(i=='4')
			  {
					flag=4;
			  }
			  
			if(i=='5')
			  {
					flag=5;
			  }
			  
			if(i=='6')
			 {
					flag=6;
			 }
		  }
      }
	  
	  
int main(void)
{
	  //�����жϽ����������շ�	
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10     
	
    /* USARTx configured as follow:
       - BaudRate = 9600 baud  ������
       - Word Length = 8 Bits  ���ݳ���
       - One Stop Bit          ֹͣλ
       - No parity             У�鷽ʽ
       - Hardware flow control disabled (RTS and CTS signals) Ӳ��������
       - Receive and transmit enabled                         ʹ�ܷ��ͺͽ���
    */
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

    //Usart1 NVIC ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	  MotorInit();
	  
	  
	  //�������ݽ����жϺ���
	  while(1)
	  {
		
		  switch(flag)
              {
					case 0:  Stop();         break  ;
					case 1:  Turnleft();     break  ;
					case 2:  Turnright();    break  ;
					case 3:  Turnback();     break  ;
					case 4:  Turnfront();    break  ;
					case 5:  Leftaround();   break  ;
					case 6:  Rightaround();  break  ;
          default: Stop();         break  ;                                                                                                
	          }		  
	  }
	  
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
 USART_SendData(USART1, (uint8_t) ch);

  /* ѭ���ȴ�ֱ�����ͽ���*/
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
