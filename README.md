# STM32 CMSIS-RTOS2 Console Library

This repository provides a simple, extensible command-line console for STM32 microcontrollers using CMSIS-RTOS2 and a UART backend.  
It enables interactive command parsing, registration of custom commands, and argument parsing for embedded applications.

---

## Features

- **UART-based command-line interface**
- **Register custom commands** with handler functions
- **Command argument parsing** (Unix-style flags and integer values, e.g. `-h 10 -v`)
- **Thread-safe UART communication** using CMSIS-RTOS2 primitives
- **Easy integration** with STM32 HAL and CMSIS-RTOS2

---

## Getting Started

### 1. Console Setup

```cpp
#include "console.h"
#include "uart.h"

// Instantiate UART and Console
Uart uart(&huart2);
// Set RTOS handles for UART (semaphore, queue, mutex) before initializing
uart.setHandleSemTx(&semTx);
uart.setHandleQueueRx(&queueRx);
uart.setHandleMutexTx(&mutexTx);
uart.init();

Console console(uart);
console.init();
```

---

### 2. Registering Commands

```cpp
// Example command handler
void cmd_led(const char* args) {
    ArgPair argList[4];
    int argc = Console::parseArgs(args, argList, 4);
    for (int i = 0; i < argc; ++i) {
        if (argList[i].flag == 's' && argList[i].value == 1) {
            // Turn LED on
        }
    }
}

// Register the command
console.registerCommand("led", cmd_led);
```

---

### 3. Running the Console Loop

Call this in your main loop or a dedicated RTOS task:

```cpp
while (1) {
    console.run(); // Receives and processes commands from UART
}
```

---

### 4. Command Argument Parsing

The console supports Unix-style flags with optional signed integer values.  
For example, the command line:  
```
led -s 1 -b 0
```
can be parsed as follows:

```cpp
ArgPair args[8];
int argc = Console::parseArgs("-s 1 -b 0", args, 8);
for (int i = 0; i < argc; ++i) {
    // args[i].flag: single char flag (e.g., 's')
    // args[i].value: integer value (or INT32_MIN if not provided)
}
```

- Flags without a value will have `value == INT32_MIN`.

---

### 5. Example CLI Session

```
> led -s 1
LED turned ON
> led -s 0
LED turned OFF
> unknown
Unknown command: unknown
```

---

## API Reference

### Registering a Command

```cpp
console.registerCommand("command", handlerFunction);
```
- `command`: Command string (e.g., `"led"`)
- `handlerFunction`: Function pointer of type `void handler(const char* args)`

### Parsing Arguments

```cpp
struct ArgPair {
    char flag;      // Single character flag, e.g., 's' for -s
    int32_t value;  // Integer value, or INT32_MIN if not present
};

static int Console::parseArgs(const char* line, ArgPair* outArgs, int maxArgs);
```

---

## Integration Notes

- The console uses a UART backend that must be initialized and configured with CMSIS-RTOS2 objects (semaphore, queue, mutex).
- The UART receive queue size is defined by `UART_RX_BUFFER_SIZE` (default: 64 bytes).
- The console supports up to 16 registered commands by default (`CONS_MAX_COMMANDS`).

---

## License

MIT License

---

## Credits

- STM32 HAL & CMSIS-RTOS2
- Inspired by classic embedded CLI