#ifndef _UART_TASK_H
#define _UART_TASK_H
#define RX_BUF_SIZE (1024)
#define TX_BUF_SIZE (1024)
#define QUEUE_SIZE (100)
#define UART_NUM (UART_NUM_0)

bool Uart_TaskInit(void);
void Uart_Task(void *params);
#endif
