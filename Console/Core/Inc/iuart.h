#ifndef IUART_H
#define IUART_H

#include <cstddef>
#include <cstdint>

// UART abstraction interface
class IUart
{
    public:
        enum class Status
        {
            NONE,
            OK,
            ERROR,
        };

        virtual ~IUart() = default;
        virtual Status transmit(const char* data, const std::size_t len) = 0;
        virtual Status receive(char* data) = 0;
};

#endif