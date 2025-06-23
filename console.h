#ifndef CONSOLE_H
#define CONSOLE_H

#include "uart.h"
#include "main.h"
#include "cmsis_os2.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <array>
#include <functional>

constexpr std::size_t CONS_MAX_COMMANDS{16};

struct ArgPair
{
    char flag;
    int32_t value;
};

class Console
{
    public:
        using CommandHandler = void(*)(const char*);

        enum class Status
        {
            NONE,
            OK,
            ERROR,
        };

        Console(Uart &uart);
        ~Console();

        Status init();

        Status registerCommand(const char *command, CommandHandler handler);
        Status run();
        Status print(const char *str);
        static int parseArgs(const char* line, ArgPair* outArgs, int maxArgs);

    private:
        void processLine(const char *line);
        void prompt();

        Uart &m_uart;
        struct CommandEntry
        {
            const char* command;
            CommandHandler handler;
        };
        std::array<CommandEntry, CONS_MAX_COMMANDS> m_commands;
        std::size_t m_commandCount;
};


#endif