#include <Console.hpp>
#include <printf.hpp>

Console::Console(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut) : ConsoleOut(ConOut)
{
}

Console::~Console()
{
}

void Console::Reset(){
    ConsoleOut->Reset(ConsoleOut, false);
}

void Console::ClearConsole(){
    ConsoleOut->ClearScreen(ConsoleOut);
}

void Console::putchar(char c){

    CHAR16 str[2];
    str[0] = (CHAR16)c;
    str[1] = 0;
    ConsoleOut->OutputString(ConsoleOut, str);

}

int Console::printf_(const char* format, ...){
    va_list args;
    va_start(args, format);

    int ret = vprintf_proxy(format, args);

    va_end(args);
    return ret;
}
