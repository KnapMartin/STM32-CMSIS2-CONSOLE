#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LOG_MSG_MAX_LEN 128
#define LOG_TIMESTAMP 1
#define LOG_QUEUE_GET_TIMEOUT 100 // ms

typedef enum
{
    LOG_TYPE_INFO,
    LOG_TYPE_WARNING,
    LOG_TYPE_ERROR
} LogType;

typedef struct
{
    char message[LOG_MSG_MAX_LEN];
    LogType type;
#if LOG_TIMESTAMP
    uint32_t timestamp_ms;
#endif
} LogMessage;


__attribute__((weak)) void LOG_write(const char* buffer);
void LOG_init(osMessageQueueId_t *queueHandle);
void LOG_INFO(const char* format, ...);
void LOG_WRN(const char* format, ...);
void LOG_ERR(const char* format, ...);
void LOG_task();

#ifdef __cplusplus
}
#endif

#endif