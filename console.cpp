#include "console.h"

Console::Console(Uart &uart)
    : m_uart(uart), m_commandCount(0)
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

Console::Status Console::registerCommand(const char *command, CommandHandler handler)
{
    if (m_commandCount >= CONS_MAX_COMMANDS)
    {
        // Optionally handle error, e.g., log or assert
        return Status::ERROR;
    }

    CommandEntry newCommand;
    newCommand.command = command;
    newCommand.handler = handler;

    m_commands[m_commandCount++] = newCommand;
    // Optionally indicate success, e.g., log or callback
    return Status::OK;
}

Console::Status Console::run()
{
    char buffer[256]; // Buffer for receiving data

    // Receive data from UART
    if (m_uart.receive(buffer) != Uart::Status::OK)
    {
        return Status::ERROR; // Reception failed
    }

    // Process the received line
    processLine(buffer);

    return Status::OK; // Run successful
}

Console::Status Console::print(const char *str)
{
    if (str == nullptr || str[0] == '\0')
    {
        return Status::NONE; // No data to print
    }

    if (m_uart.transmit(const_cast<char *>(str), strlen(str)) != Uart::Status::OK)
    {
        return Status::ERROR; // Transmission failed
    }

    return Status::OK; // Print successful
}

void Console::processLine(const char *line)
{
    // Check if the line is empty
    if (line == nullptr || line[0] == '\0')
    {
        prompt();
        return;
    }

    // Iterate through registered commands to find a match
    for (std::size_t i = 0; i < m_commandCount; ++i)
    {
        const char *command = m_commands[i].command;
        CommandHandler handler = m_commands[i].handler;

        size_t cmdLen = strlen(command);
        if (strncmp(line, command, cmdLen) == 0)
        {
            handler(line); // Call the command handler
            prompt();      // Prompt for the next command
            return;
        }
    }

    // If no command matched, print an error message
    const char errorMsg[] = "Unknown command: ";
    print(errorMsg);
    print(line);
    print("\r\n");
    prompt(); // Prompt for the next command
}

void Console::prompt()
{
    const char promptMsg[] = "> ";
    print(promptMsg); // Print the prompt message
    // print("\r\n"); // Print a new line after error message
}

int Console::parseArgs(const char* line, ArgPair* outArgs, int maxArgs)
{
    int count = 0;
    const char* p = line;
    while (*p && count < maxArgs)
    {
        // Skip whitespace
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == '-' && *(p + 1) && *(p + 1) != ' ' && *(p + 1) != '\t')
        {
            char flagChar = *(p + 1);
            p += 2;
            // Skip whitespace
            while (*p == ' ' || *p == '\t') ++p;
            int32_t val = INT32_MIN; // Sentinel for "no value"
            if (*p && *p != '-')
            {
                char* endptr;
                val = strtol(p, &endptr, 10);
                if (endptr != p)
                {
                    p = endptr;
                }
            }
            outArgs[count].flag = flagChar;
            outArgs[count].value = val;
            ++count;
        }
        else
        {
            // Skip non-flag tokens
            while (*p && *p != ' ' && *p != '\t') ++p;
        }
    }
    return count;
}
