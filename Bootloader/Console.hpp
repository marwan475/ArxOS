#pragma once

#include <uefi.hpp>

class Console
{
private:
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConsoleOut;
public:
    Console(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut);
    void Reset();
    void ClearConsole();
    void putchar(char c);
    int printf_(const char* format, ...);
    ~Console();
};

