
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
#define CTL_CMD_CONFIG_USART1   0x21
#define CTL_CMD_CONFIG_USART2   0x22
#define CTL_CMD_CONFIG_USART3   0x23

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
TUINT8 u8Length;
tyun_AnyData unAnyData;
TUINT32 ctl_USART_Dynamic_Baudrate;



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



		u8Length = SPI1_RxBuf[1];


		switch (SPI1_RxBuf[0])
		{
			case CTL_CMD_USART1_TX:		if (u8Length <= SPI1_TXRX_BUFFER_SIZE)
										{
											for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
											{
												BUF1_SndBuf_Put(SPI1_RxBuf[i]);
											}

											if (!BUF1_u8SndBuf_Empty())
											{
												u8Temp = BUF1_u8SndBuf_Get();
												HAL_UART_Transmit_IT(&huart1, &u8Temp, 1);
											}
										}

										break;

			case CTL_CMD_USART2_TX:		if (u8Length <= SPI1_TXRX_BUFFER_SIZE)
										{
											for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
											{
												BUF2_SndBuf_Put(SPI1_RxBuf[i]);
											}

											if (!BUF2_u8SndBuf_Empty())
											{
												u8Temp = BUF2_u8SndBuf_Get();
												HAL_UART_Transmit_IT(&huart2, &u8Temp, 1);
											}
										}

										break;

			case CTL_CMD_USART3_TX:		if (u8Length <= SPI1_TXRX_BUFFER_SIZE)
										{
											for (int i = 2; i < SPI1_RxBuf[1] + 2; i++)
											{
												BUF3_SndBuf_Put(SPI1_RxBuf[i]);
											}

											if (!BUF3_u8SndBuf_Empty())
											{
												u8Temp = BUF3_u8SndBuf_Get();
												HAL_UART_Transmit_IT(&huart3, &u8Temp, 1);
											}
										}

										break;


			case CTL_CMD_CONFIG_USART1:		memcpy((char*)unAnyData.au8Data, (char*)&SPI1_RxBuf[1], 4);

											ctl_USART_Dynamic_Baudrate = unAnyData.u32Data;


											huart1.Instance = USART1;
											huart1.Init.BaudRate = ctl_USART_Dynamic_Baudrate;
											huart1.Init.WordLength = UART_WORDLENGTH_8B;
											huart1.Init.StopBits = UART_STOPBITS_1;
											huart1.Init.Parity = UART_PARITY_NONE;
											huart1.Init.Mode = UART_MODE_TX_RX;
											huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
											huart1.Init.OverSampling = UART_OVERSAMPLING_16;
											if (HAL_UART_Init(&huart1) != HAL_OK)
											{
											  Error_Handler();
											}

											break;



			case CTL_CMD_CONFIG_USART2:		memcpy((char*)unAnyData.au8Data, (char*)&SPI1_RxBuf[1], 4);

											ctl_USART_Dynamic_Baudrate = unAnyData.u32Data;


											huart2.Instance = USART2;
											huart2.Init.BaudRate = ctl_USART_Dynamic_Baudrate;
											huart2.Init.WordLength = UART_WORDLENGTH_8B;
											huart2.Init.StopBits = UART_STOPBITS_1;
											huart2.Init.Parity = UART_PARITY_NONE;
											huart2.Init.Mode = UART_MODE_TX_RX;
											huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
											huart2.Init.OverSampling = UART_OVERSAMPLING_16;
											if (HAL_UART_Init(&huart2) != HAL_OK)
											{
											  Error_Handler();
											}

											break;



			case CTL_CMD_CONFIG_USART3:		memcpy((char*)unAnyData.au8Data, (char*)&SPI1_RxBuf[1], 4);

											ctl_USART_Dynamic_Baudrate = unAnyData.u32Data;


											huart3.Instance = USART3;
											huart3.Init.BaudRate = ctl_USART_Dynamic_Baudrate;
											huart3.Init.WordLength = UART_WORDLENGTH_8B;
											huart3.Init.StopBits = UART_STOPBITS_1;
											huart3.Init.Parity = UART_PARITY_NONE;
											huart3.Init.Mode = UART_MODE_TX_RX;
											huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
											huart3.Init.OverSampling = UART_OVERSAMPLING_16;
											if (HAL_UART_Init(&huart3) != HAL_OK)
											{
											  Error_Handler();
											}

											break;

			default: break;
		}



		HAL_SPI_TransmitReceive_DMA(&hspi1, SPI1_TxBuf, SPI1_RxBuf, SPI1_TXRX_BUFFER_SIZE);

	}

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
