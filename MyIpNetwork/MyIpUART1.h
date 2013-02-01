
#ifndef _MYIPUART1_H_
#define _MYIPUART1_H_

#include "GenericTypeDefs.h"

void MyIpInit_UART1(BYTE s);
void MyIpService_UART1(BYTE s);
BOOL MyIpThreadSafeSendPacketCheck_UART1(BYTE s, BOOL doClearFlag);
int MyIpThreadSafeReadBufferHead_UART1(BYTE s);
void MyIpProcessRxData_UART1(BYTE s);
void MyIpOnConnect_UART1(BYTE s);


#endif // _MYIPUART1_H_
