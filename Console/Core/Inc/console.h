#ifndef CONSOLE_H
#define CONSOLE_H

#include "uart.h"
#include "main.h"
#include "cmsis_os2.h"
#include <string>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <array>
#include <tuple>
#include <functional>

constexpr std::size_t MAX_COMMANDS{16};

class Console
{
    public:
        using CommandHandler = std::function<void(const std::string&)>;

        enum class Status
        {
            NONE,
            OK,
            ERROR,
        };

        Console(Uart &uart);
        ~Console();

        Status init();

        Status registerCommand(const std::string &command, CommandHandler handler);
        Status run();
        Status print(std::string &str);

    private:
        void processLine(const std::string &line);
        void prompt();

        Uart &m_uart;
        std::array< std::tuple<std::string, CommandHandler>, MAX_COMMANDS > m_commands;
        std::size_t m_commandCount;
};


#endif