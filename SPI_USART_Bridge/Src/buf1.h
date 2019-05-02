
/******************************************************************************
*  File           :  buf1.h
*  Version        :  1
*  Last Changes   :  2019-04-29 19:56:38
******************************************************************************/

#ifndef __BUF1_H
#define __BUF1_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*  Include Files
******************************************************************************/
#include "typ.h"

/******************************************************************************
*  Global Constants
******************************************************************************/

#define BUF1_SNDLEN 		1024
#define BUF1_RECLEN 		1024

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

extern TUINT8 BUF1_u8Buf;
/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

void BUF1_SndBuf_Put (TUINT8 u8Data);
TUINT8 BUF1_u8SndBuf_Get (void);
TUINT8 BUF1_u8SndBuf_Empty (void);

void BUF1_RecBuf_Put (TUINT8 u8Data);
TUINT8 BUF1_u8RecBuf_Get (void);
TUINT8 BUF1_u8RecBuf_Empty (void);

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __BUF1_H */

/******************************************************************************
*  END OF FILE    :  buf1.h
******************************************************************************/
