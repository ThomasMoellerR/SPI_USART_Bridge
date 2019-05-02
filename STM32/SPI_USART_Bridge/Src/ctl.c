
/******************************************************************************
*  File           :  ctl.c
*  Version        :  1
*  Last Changes   :  2019-04-30 17:01:37
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include "main.h"
#include "typ.h"
#include "ctl.h"
#include "buf1.h"
#include "buf2.h"
#include "buf3.h"
#include "spi1.h"

#include <string.h>

/******************************************************************************
*  Local Constants
******************************************************************************/

#define CTL_CMD_USART1_TX		0x11
#define CTL_CMD_USART2_TX		0x12
#define CTL_CMD_USART3_TX		0x13

/******************************************************************************
*  Local Type Definitions
******************************************************************************/

/******************************************************************************
*  Local Variables
******************************************************************************/

TUINT8 ctl_USART1_TxCounter = 0;
TUINT8 ctl_USART2_TxCounter = 0;
TUINT8 ctl_USART3_TxCounter = 0;
TUINT8 ctl_SPI1_TxCounter = 0;
TUINT8 ctl_SPI1_TxCounter_Backup = 0;
TUINT8 ctl_Bytes_Read_From_Queue = 0;

TUINT8 u8Temp;


enum {CTL_USART1, CTL_USART2, CTL_USART3} ctl_State = CTL_USART1;

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_tx;


/******************************************************************************
*  Global Variables
******************************************************************************/

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

/******************************************************************************
*  Local Functions
******************************************************************************/

/******************************************************************************
*  Global Functions
******************************************************************************/

/******************************************************************************
*  Function Name  :  
*  Description    :  
*  Parameter(s)   :  
*  Return Value   :  
******************************************************************************/

void CTL_Ini (void)
{
	HAL_UART_Receive_IT(&huart1, &BUF1_u8Buf,1);
	HAL_UART_Receive_IT(&huart2, &BUF2_u8Buf,1);
	HAL_UART_Receive_IT(&huart3, &BUF3_u8Buf,1);

	//HAL_SPI_TransmitReceive_DMA(&hspi1, SPI1_TxBuf, SPI1_RxBuf, SPI1_TXRX_BUFFER_SIZE);

	memset(SPI1_TxBuf, 0, sizeof(SPI1_TxBuf));
	memset(SPI1_RxBuf, 0, sizeof(SPI1_RxBuf));
}





/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void CTL_Main (void)
{
	if (SPI1_TxCompleted)
	{
		SPI1_TxCompleted = 0;
		ctl_SPI1_TxCounter = 0;

		memset(SPI1_TxBuf, 0, sizeof(SPI1_TxBuf));

		switch(ctl_State)
		{
			case CTL_USART1:	if (!BUF1_u8RecBuf_Empty())
								{
									SPI1_TxBuf[ctl_SPI1_TxCounter++] = CTL_CMD_USART1_TX; // Cmd

									ctl_SPI1_TxCounter += 1; // Skip the length byte. It depends on how many byte read from queue

									ctl_Bytes_Read_From_Queue = 0;

									while (!BUF1_u8RecBuf_Empty())
									{
										SPI1_TxBuf[ctl_SPI1_TxCounter] = BUF1_u8RecBuf_Get(); // Data
										ctl_SPI1_TxCounter++;
										ctl_Bytes_Read_From_Queue++;

										if (ctl_SPI1_TxCounter == SPI1_TXRX_BUFFER_SIZE) break;
									}

									SPI1_TxBuf[ctl_SPI1_TxCounter - ctl_Bytes_Read_From_Queue - 1] = ctl_Bytes_Read_From_Queue; // Length
								}

								ctl_State = CTL_USART2;

								break;

			case CTL_USART2:	if (!BUF2_u8RecBuf_Empty())
								{
									SPI1_TxBuf[ctl_SPI1_TxCounter++] = CTL_CMD_USART2_TX; // Cmd

									ctl_SPI1_TxCounter += 1; // Skip the length byte. It depends on how many byte read from queue

									ctl_Bytes_Read_From_Queue = 0;

									while (!BUF2_u8RecBuf_Empty())
									{
										SPI1_TxBuf[ctl_SPI1_TxCounter] = BUF2_u8RecBuf_Get(); // Data
										ctl_SPI1_TxCounter++;
										ctl_Bytes_Read_From_Queue++;

										if (ctl_SPI1_TxCounter == SPI1_TXRX_BUFFER_SIZE) break;
									}

									SPI1_TxBuf[ctl_SPI1_TxCounter - ctl_Bytes_Read_From_Queue - 1] = ctl_Bytes_Read_From_Queue; // Length
								}

								ctl_State = CTL_USART3;

								break;

			case CTL_USART3:	if (!BUF3_u8RecBuf_Empty())
								{
									SPI1_TxBuf[ctl_SPI1_TxCounter++] = CTL_CMD_USART3_TX; // Cmd

									ctl_SPI1_TxCounter += 1; // Skip the length byte. It depends on how many byte read from queue

									ctl_Bytes_Read_From_Queue = 0;

									while (!BUF3_u8RecBuf_Empty())
									{
										SPI1_TxBuf[ctl_SPI1_TxCounter] = BUF3_u8RecBuf_Get(); // Data
										ctl_SPI1_TxCounter++;
										ctl_Bytes_Read_From_Queue++;

										if (ctl_SPI1_TxCounter == SPI1_TXRX_BUFFER_SIZE) break;
									}

									SPI1_TxBuf[ctl_SPI1_TxCounter - ctl_Bytes_Read_From_Queue - 1] = ctl_Bytes_Read_From_Queue; // Length
								}

								ctl_State = CTL_USART1;

								break;

			default:			break;
		}


		switch (SPI1_RxBuf[0])
		{
			case CTL_CMD_USART1_TX:		for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
										{
											BUF1_SndBuf_Put(SPI1_RxBuf[i]);
										}


										if (!BUF1_u8SndBuf_Empty())
										{
											u8Temp = BUF1_u8SndBuf_Get();
											HAL_UART_Transmit_IT(&huart1, &u8Temp, 1);
										}

										break;

			case CTL_CMD_USART2_TX:		for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
										{
											BUF2_SndBuf_Put(SPI1_RxBuf[i]);
										}

										if (!BUF2_u8SndBuf_Empty())
										{
											u8Temp = BUF2_u8SndBuf_Get();
											HAL_UART_Transmit_IT(&huart2, &u8Temp, 1);
										}

										break;

			case CTL_CMD_USART3_TX:		for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
										{
											BUF3_SndBuf_Put(SPI1_RxBuf[i]);
										}

										if (!BUF3_u8SndBuf_Empty())
										{
											u8Temp = BUF3_u8SndBuf_Get();
											HAL_UART_Transmit_IT(&huart3, &u8Temp, 1);
										}

										break;

			default: break;
		}





		// Then Transmit
		HAL_SPI_TransmitReceive_DMA(&hspi1, SPI1_TxBuf, SPI1_RxBuf, SPI1_TXRX_BUFFER_SIZE);

	}















	/*
	  HAL_SPI_Transmit_DMA(&hspi1, spi_tx_buff, 20);

	  HAL_UART_Transmit_DMA(&huart1,tx_buff,10);

	  HAL_Delay(3000);
	  */
	 // HAL_UART_Receive_IT(&huart2, rx_buff, 1);

}

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ctl.h
******************************************************************************/
