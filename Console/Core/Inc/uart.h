#ifndef UART_H
#define UART_H

#include "main.h"
#include "iuart.h"
#include "cmsis_os2.h"
#include <cstdint>

#define RX_BUFFER_SIZE 32 // Size of the receive buffer

class Uart : public IUart
{
    public:
        Uart(UART_HandleTypeDef *huart);
        ~Uart() override;

        void setHandleSemTx(osSemaphoreId_t *semTx) {m_semTx = semTx;}
        void setHandleQueueRx(osMessageQueueId_t *queueRx) {m_queueRx = queueRx;}
        void setHandleMutexTx(osMutexId_t *mutexTx) {m_mutexTx = mutexTx;}
        IUart::Status init(); 
        IUart::Status transmit(char* data, const std::size_t len) override;
        IUart::Status receive(char* data, const std::size_t len) override;
        IUart::Status handleRxInterrupt(UART_HandleTypeDef *huart);
        IUart::Status handleTxInterrupt(UART_HandleTypeDef *huart);

    private:
        volatile uint8_t m_rxChar;
        UART_HandleTypeDef *m_huart;
        osSemaphoreId_t *m_semTx;
        osMessageQueueId_t *m_queueRx;
        osMutexId_t *m_mutexTx;
};



#endif