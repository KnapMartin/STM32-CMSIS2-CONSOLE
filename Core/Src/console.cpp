#include "console.h"

Console::Console(Uart &uart)
    : m_uart(uart)
    , m_commandCount(0)
{
}

Console::~Console()
{
}

Console::Status Console::init()
{
    if (!m_uart.isInitialized())
    {
        return Status::ERROR; // UART not initialized
    }

    m_commandCount = 0;

    prompt();

    return Status::OK; // Initialization successful
}

Console::Status Console::registerCommand(const std::string &command, CommandHandler handler, const std::size_t matchLength)
{
    if (m_commandCount >= MAX_COMMANDS)
    {
        // Optionally handle error, e.g., log or assert
        return Status::ERROR;
    }

    m_commands[m_commandCount++] = std::make_tuple(command, handler, matchLength);
    // Optionally indicate success, e.g., log or callback
    return Status::OK;
}

Console::Status Console::run()
{
    char buffer[256]; // Buffer for receiving data
    std::size_t len = 0;

    // Receive data from UART
    if (m_uart.receive(buffer, sizeof(buffer)) != IUart::Status::OK)
    {
        return Status::ERROR; // Reception failed
    }

    // Process the received line
    std::string line(buffer, len);
    processLine(line);

    return Status::OK; // Run successful
}

Console::Status Console::print(std::string &str)
{
    if (str.empty())
    {
        return Status::NONE; // No data to print
    }

    if (m_uart.transmit(str.data(), str.size()) != IUart::Status::OK)
    {
        return Status::ERROR; // Transmission failed
    }

    return Status::OK; // Print successful
}

void Console::processLine(const std::string &line)
{
    // Check if the line is empty
    if (line.empty())
    {
        prompt();
        return;
    }

    // Find the command in the registered commands
    for (std::size_t i = 0; i < m_commandCount; ++i)
    {
        const auto &[command, handler, matchLength] = m_commands[i];
        if (line.compare(0, matchLength, command) == 0)
        {
            handler(line); // Call the command handler
            prompt(); // Prompt for the next command
            return;
        }
    }

    // If no command matched, print an error message
    std::string errorMsg = "Unknown command: " + line + "\n";
    print(errorMsg);
    prompt(); // Prompt for the next command
}

void Console::prompt()
{
    std::string promptMsg = "> ";
    print(promptMsg); // Print the prompt message
}
