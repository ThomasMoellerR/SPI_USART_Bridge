
/******************************************************************************
*  File           :  spi1.h
*  Version        :  1
*  Last Changes   :  2019-04-30 16:53:59
******************************************************************************/

#ifndef __SPI1_H
#define __SPI1_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*  Include Files
******************************************************************************/

/******************************************************************************
*  Global Constants
******************************************************************************/

#define SPI1_TXRX_BUFFER_SIZE 10

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

extern TUINT8 SPI1_TxCompleted;
extern TUINT8 SPI1_RxBuf[SPI1_TXRX_BUFFER_SIZE];
extern TUINT8 SPI1_TxBuf[SPI1_TXRX_BUFFER_SIZE];

/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

extern void SPI1_Ini (void);

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __SPI1_H */

/******************************************************************************
*  END OF FILE    :  spi1.h
******************************************************************************/
