#pragma once

#include <iostream>
#include <string_view>
#include <format>
#include <Windows.h>
#include <shared_mutex>

enum class message_type : std::uint32_t {
    none = 0,
    success = 10,
    debug = 9,		
    warning = 14,	
    prompt = 11,
    error = 12		
};

inline std::ostream& operator<< (std::ostream& os, const message_type type)
{
    switch (type)
    {
    case message_type::debug:	return os << ".";
    case message_type::success:	return os << "+";
    case message_type::error:	return os << "!";
    case message_type::prompt:	return os << ">";
    case message_type::warning:		return os << "*";
    default: return os << "";
    }
}

class c_logger {
public:
    void initialize() {
        AllocConsole();

        FILE* conin, * conout;

        freopen_s(&conin, "conin$", "r", stdin);
        freopen_s(&conout, "conout$", "w", stdout);
        freopen_s(&conout, "conout$", "w", stderr);

        std::cout << R"( 
       !
      .-.
    __|=|__
   (_/`-`\_)         tuzeynah
   //\___/\\             @
   <>/   \<> https://github.com/tuzeynah
    \|_._|/
     <_I_>
      |||
jgs  /_|_\

)" << std::endl;
    }

    void shutdown() {
        FreeConsole();
    }

    void print(const message_type type, std::string_view func, std::string_view message) {
        const auto console = GetStdHandle(STD_OUTPUT_HANDLE);
        std::unique_lock<decltype(mutex)> lock(mutex);

        if (type != message_type::none) {
            SetConsoleTextAttribute(console, 7);
            std::cout << "[ ";

            SetConsoleTextAttribute(console, (WORD)(type));
            std::cout << type;

            SetConsoleTextAttribute(console, 7);
            std::cout << " ] ";
        }

#ifdef _DEBUG
        SetConsoleTextAttribute(console, 15);
        std::cout << "[ ";

        SetConsoleTextAttribute(console, (WORD)(type));
        std::cout << func;

        SetConsoleTextAttribute(console, 15);
        std::cout << " ] ";
#endif

        if (type == message_type::debug)
            SetConsoleTextAttribute(console, 8);
        else
            SetConsoleTextAttribute(console, 7);


        std::cout << message << "\n";
    }
private:
    std::shared_timed_mutex mutex{};
};

inline auto logger = c_logger();

#define log_debug(...)    logger.print(message_type::debug,   __FUNCTION__, __VA_ARGS__)
#define log_ok(...)      logger.print(message_type::success, __FUNCTION__, __VA_ARGS__)
#define log_err(...)     logger.print(message_type::error,   __FUNCTION__, __VA_ARGS__)
#define log_prompt(...)  logger.print(message_type::prompt,  __FUNCTION__, __VA_ARGS__)
#define log_warn(...)    logger.print(message_type::warning, __FUNCTION__, __VA_ARGS__)
#define log_raw(...)     logger.print(message_type::none,    __FUNCTION__, __VA_ARGS__)