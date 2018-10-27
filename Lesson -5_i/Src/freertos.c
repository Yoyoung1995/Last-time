/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "public.h"
#include "Far_ModifyTask_Resources.h"
#include "GPRS_DataSendTask_Resources.h"
#include "DS18b20Module.h"
#include "Temp_ControlTask_Resources.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId Far_ModifyHandle;
osThreadId USART1_ServeHandle;
osThreadId USART2_ServeHandle;
osThreadId USART3_ServeHandle;
osThreadId GPRS_DataSendHandle;
osThreadId Temp_ControlHandle;
osTimerId Timer_GPRS_DataSendHandle;
osTimerId Timer_Temp_ControlHandle;
osSemaphoreId bSem_USART1_ServeHandle;
osSemaphoreId bSem_USART2_ServeHandle;
osSemaphoreId bSem_USART3_ServeHandle;
osSemaphoreId bSem_GPRS_DataSendHandle;
osSemaphoreId bSem_Temp_ControlHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void Far_ModifyTask(void const * argument);
void USART1_Serve_Task(void const * argument);
void USART2_Serve_Task(void const * argument);
void USART3_Serve_Task(void const * argument);
void GPRS_DataSendTask(void const * argument);
void Temp_ControlTask(void const * argument);
void Timer_GPRS_DataSendCallback(void const * argument);
void Timer_Temp_ControlCallback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of bSem_USART1_Serve */
  osSemaphoreDef(bSem_USART1_Serve);
  bSem_USART1_ServeHandle = osSemaphoreCreate(osSemaphore(bSem_USART1_Serve), 1);

  /* definition and creation of bSem_USART2_Serve */
  osSemaphoreDef(bSem_USART2_Serve);
  bSem_USART2_ServeHandle = osSemaphoreCreate(osSemaphore(bSem_USART2_Serve), 1);

  /* definition and creation of bSem_USART3_Serve */
  osSemaphoreDef(bSem_USART3_Serve);
  bSem_USART3_ServeHandle = osSemaphoreCreate(osSemaphore(bSem_USART3_Serve), 1);

  /* definition and creation of bSem_GPRS_DataSend */
  osSemaphoreDef(bSem_GPRS_DataSend);
  bSem_GPRS_DataSendHandle = osSemaphoreCreate(osSemaphore(bSem_GPRS_DataSend), 1);

  /* definition and creation of bSem_Temp_Control */
  osSemaphoreDef(bSem_Temp_Control);
  bSem_Temp_ControlHandle = osSemaphoreCreate(osSemaphore(bSem_Temp_Control), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of Timer_GPRS_DataSend */
  osTimerDef(Timer_GPRS_DataSend, Timer_GPRS_DataSendCallback);
  Timer_GPRS_DataSendHandle = osTimerCreate(osTimer(Timer_GPRS_DataSend), osTimerPeriodic, NULL);

  /* definition and creation of Timer_Temp_Control */
  osTimerDef(Timer_Temp_Control, Timer_Temp_ControlCallback);
  Timer_Temp_ControlHandle = osTimerCreate(osTimer(Timer_Temp_Control), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	osTimerStart(Timer_GPRS_DataSendHandle,5000);
	osTimerStart(Timer_Temp_ControlHandle,1000);
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of Far_Modify */
  osThreadDef(Far_Modify, Far_ModifyTask, osPriorityNormal, 0, 128);
  Far_ModifyHandle = osThreadCreate(osThread(Far_Modify), NULL);

  /* definition and creation of USART1_Serve */
  osThreadDef(USART1_Serve, USART1_Serve_Task, osPriorityRealtime, 0, 128);
  USART1_ServeHandle = osThreadCreate(osThread(USART1_Serve), NULL);

  /* definition and creation of USART2_Serve */
  osThreadDef(USART2_Serve, USART2_Serve_Task, osPriorityRealtime, 0, 128);
  USART2_ServeHandle = osThreadCreate(osThread(USART2_Serve), NULL);

  /* definition and creation of USART3_Serve */
  osThreadDef(USART3_Serve, USART3_Serve_Task, osPriorityRealtime, 0, 128);
  USART3_ServeHandle = osThreadCreate(osThread(USART3_Serve), NULL);

  /* definition and creation of GPRS_DataSend */
  osThreadDef(GPRS_DataSend, GPRS_DataSendTask, osPriorityNormal, 0, 128);
  GPRS_DataSendHandle = osThreadCreate(osThread(GPRS_DataSend), NULL);

  /* definition and creation of Temp_Control */
  osThreadDef(Temp_Control, Temp_ControlTask, osPriorityNormal, 0, 128);
  Temp_ControlHandle = osThreadCreate(osThread(Temp_Control), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	Far_Modify.xPointerQueue = xQueueCreate(5,sizeof(void *));
	GPRS_DataSend.xPointerQueue = xQueueCreate(1,sizeof(void *));
  /* USER CODE END RTOS_QUEUES */
}

/* Far_ModifyTask function */
void Far_ModifyTask(void const * argument)
{

  /* USER CODE BEGIN Far_ModifyTask */
	USART_RECEIVETYPE * pMail =NULL;
	
	uint16_t DataLen = 0;		//数据内容长度
	uint16_t FrameCRC = 0;  //邮件中的CRC值
	uint8_t  RegisterNum = 0;  //操作寄存器的个数
	uint16_t Addr_Temp = 0;	//临时地址值
	uint8_t  ack[2] = {0x80,0x02};	
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(Far_Modify.xPointerQueue,		//等待消息队列
									&pMail,
									portMAX_DELAY);
		//--------处理从下面开始
						//包头+对象正确 + CRC校验正确 后，视为数据正确，根据字节长度反算操作的寄存器个数，循环进行改值操作
		if( (pMail->RX_pData[0]==0x55) && (pMail->RX_pData[1]==0xAA) && (pMail->RX_pData[2]==0x02))
		{
						//CRC
			DataLen = pMail->RX_pData[10] + (pMail->RX_pData[11]<<8);
			FrameCRC = pMail->RX_pData[DataLen+12] + ( pMail->RX_pData[DataLen+13] <<8 );
			if( FrameCRC == gprsCRC( &(pMail->RX_pData[2]), DataLen+10 ) )
			{
				RegisterNum = ( DataLen -1)/4;
				for(uint8_t i=0;i<RegisterNum;i++)	//循环改写节能板寄存器的值
				{
					Addr_Temp = pMail->RX_pData[13+4*i] + ( pMail->RX_pData[14+4*i]<<8 );
					if(Addr_Temp <=10 )  //防止内存溢出 或 到达不可写区域
						//改写节能控制板寄存器
					BoardRegister[Addr_Temp] = 	pMail->RX_pData[15+4*i] + ( pMail->RX_pData[16+4*i]<<8 );		
				}
				//返回0x80 + 0x02
				USART3_485_Send(ack,2);
			}
		}
		//---------处理结束
		vPortFree(pMail);
		pMail = NULL;
		//----------Done
  }
  /* USER CODE END Far_ModifyTask */
}

/* USART1_Serve_Task function */
void USART1_Serve_Task(void const * argument)
{
  /* USER CODE BEGIN USART1_Serve_Task */
  /* Infinite loop */
	/**
		这是一个串口1中断服务任务 ，
		为串口1接收到的数据提供处理服务
	**/
  for(;;)
  {
		osSemaphoreWait(bSem_USART1_ServeHandle,osWaitForever);		
		
		if(UsartType_1.RX_flag)			//receive flag
			UsartType_1.RX_flag = 0;			//clean flag
		
		//----------  处理从下面开始 --------------
		

  }
  /* USER CODE END USART1_Serve_Task */
}

/* USART2_Serve_Task function */
void USART2_Serve_Task(void const * argument)
{
  /* USER CODE BEGIN USART2_Serve_Task */
	USART_RECEIVETYPE * pMail = NULL;
  /* Infinite loop */
	/**
		这是一个串口2中断服务任务 ，
		为串口2接收到的数据提供处理服务
		串口2接收到的只有从机的响应数据，简单判断从机设备号后统统扔入GPRS_DataSendTask任务内建消息队列
	**/
  for(;;)
  {
		osSemaphoreWait(bSem_USART2_ServeHandle,osWaitForever);		
		
		if(UsartType_2.RX_flag)			//receive flag
			UsartType_2.RX_flag = 0;			//clean flag
		
		//----------  处理从下面开始 --------------
		if( ( UsartType_2.RX_pData[0] != 1)&&(UsartType_2.RX_pData[0] != 63) )			//先简单判断
				continue;
		if(  UsartType_2.RX_pData[1] != 3 )		//屏蔽掉其它码回传的数据帧
				continue;
		if( uxQueueSpacesAvailable(GPRS_DataSend.xPointerQueue)>=1 )					//队列有没有满？
				pMail = pvPortMalloc(sizeof(USART_RECEIVETYPE));		//申请动态内存
		if( pMail != NULL )				//动态内存申请成功了？
		{
				memcpy(pMail,&UsartType_2,sizeof(UsartType_2));		//内存拷贝
				xQueueSend(GPRS_DataSend.xPointerQueue,		//扔入消息队列
								 &pMail,
								 portMAX_DELAY );		
		}
		pMail = NULL;
		
  }
  /* USER CODE END USART2_Serve_Task */
}

/* USART3_Serve_Task function */
void USART3_Serve_Task(void const * argument)
{
  /* USER CODE BEGIN USART3_Serve_Task */
	USART_RECEIVETYPE * pMail =NULL;
  /* Infinite loop */
	/**
		这是一个串口3中断服务任务 ，
		为串口3接收到的数据提供处理服务
		接收来自GPRS的数据，仍入任务内建的消息队列
	**/
  for(;;)
  {
		osSemaphoreWait(bSem_USART3_ServeHandle,osWaitForever);		
		
		if(UsartType_3.RX_flag)			//receive flag
			UsartType_3.RX_flag = 0;			//clean flag
		
		//----------  处理从下面开始 --------------
		if( uxQueueSpacesAvailable(Far_Modify.xPointerQueue)>=1 )			//查询队列有没有满？剩余的项数
			pMail = pvPortMalloc(sizeof(USART_RECEIVETYPE));	//申请动态内存，返回指针
		if(pMail != NULL)
		{
			memcpy(pMail,&UsartType_3,sizeof(UsartType_3));				//内存拷贝
			xQueueSend(Far_Modify.xPointerQueue,
								 &pMail,
								 portMAX_DELAY );
		}
		pMail = NULL;
  }
  /* USER CODE END USART3_Serve_Task */
}

/* GPRS_DataSendTask function */
void GPRS_DataSendTask(void const * argument)
{
  /* USER CODE BEGIN GPRS_DataSendTask */
	USART_RECEIVETYPE * pMail = NULL;
  /* Infinite loop */
  for(;;)
  {
		osSemaphoreWait(bSem_GPRS_DataSendHandle,osWaitForever);
		
		Modbus_03_Search( 1, 1199,28);		//读外机 1200~1227地址的寄存器
		
		xQueueReceive(GPRS_DataSend.xPointerQueue,		//等待消息队列  500个Ticks
									&pMail,
									1000);		
		
		Modbus_03_Search( 1, 5103,19);		//读外机 5104~5122地址的寄存器
		
		Modbus_Modify(pMail);				//参数修改
		vPortFree(pMail);		//动态内存释放
		pMail = NULL;				
		
 		xQueueReceive(GPRS_DataSend.xPointerQueue,		//等待消息队列  500个Ticks
									&pMail,
									1000);	   
		
		osDelay(30);		//( 给数字电表留点处理时间，因为 否则 在数字电表看来，总线上的两串数据帧是连在一起的 )
		
		Modbus_03_Search( 63, 0x2000,16);		//读电表 2000H~200FH地址的寄存器
		
		Modbus_Modify(pMail);					//参数修改
		vPortFree(pMail);		//动态内存释放
		pMail = NULL;					

 		xQueueReceive(GPRS_DataSend.xPointerQueue,		//等待消息队列  500个Ticks
									&pMail,
									500);	    

		Modbus_03_Search( 63, 0x4000,2);		//读电表 4000H~4001H地址的寄存器
		
		Modbus_Modify(pMail);					//参数修改
		vPortFree(pMail);		//动态内存释放
		pMail = NULL;		

 		xQueueReceive(GPRS_DataSend.xPointerQueue,		//等待消息队列  500个Ticks
									&pMail,
									500);	  
									
		Modbus_Modify(pMail);					//参数修改
		vPortFree(pMail);		//动态内存释放
		pMail = NULL;		

		GPRS_Send(theMAC,DeviceNum);			//向GPRS上传节能控制板寄存器信息
  }
  /* USER CODE END GPRS_DataSendTask */
}

/* Temp_ControlTask function */
void Temp_ControlTask(void const * argument)
{
  /* USER CODE BEGIN Temp_ControlTask */
  /* Infinite loop */
  for(;;)
  {
		osSemaphoreWait(bSem_Temp_ControlHandle,osWaitForever);
		DS18b20_FinalTemp = ReadTemperature();
		//恒温控制
		switch(BoardRegister[1])
		{
			case 1 :				//关机
				Modbus_SetRegister(1,999,0);
				break;
			case 2 :				//制热
				HeatAir_Running();
				break;
			case 3 :				//制冷
				CoolAir_Running();
				break;
			default : 			
				break;
		}
		//电能表清零
		if( BoardRegister[3])
		{
			Modbus_SetRegister(63,2,1);
		}
		//done
  }
  /* USER CODE END Temp_ControlTask */
}

/* Timer_GPRS_DataSendCallback function */
void Timer_GPRS_DataSendCallback(void const * argument)
{
  /* USER CODE BEGIN Timer_GPRS_DataSendCallback */
  osSemaphoreRelease(bSem_GPRS_DataSendHandle);
  /* USER CODE END Timer_GPRS_DataSendCallback */
}

/* Timer_Temp_ControlCallback function */
void Timer_Temp_ControlCallback(void const * argument)
{
  /* USER CODE BEGIN Timer_Temp_ControlCallback */
  osSemaphoreRelease(bSem_Temp_ControlHandle);
  /* USER CODE END Timer_Temp_ControlCallback */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
