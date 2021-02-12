#ifndef MEMORYLIB
#define MEMORYLIB

#include <windows.h>
#include <string>

class MemoryLib
{
    public:
    static inline STARTUPINFOA _sInfo;
    static inline PROCESS_INFORMATION _pInfo;

    static void ExecuteProcess(std::string InputName)
    {
        ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
        ZeroMemory(&_pInfo, sizeof(_pInfo));

        CreateProcessA(InputName.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &_sInfo, &_pInfo);
    };

    static std::vector<char> ReadBytes(int Address, int Length)
    {
        std::vector<char> _buffer;
        _buffer.resize(Length);

        ReadProcessMemory(_pInfo.hProcess, (void*)Address, _buffer.data(), Length, 0);
        return _buffer;
    }

    static uint16_t ReadShort(int Address)
    {
        auto _buffer = ReadBytes(Address, 2);
        return (_buffer[1] << 8) | _buffer[2];
    }

    static uint32_t ReadInt(int Address)
    {
        auto _buffer = ReadBytes(Address, 4);
        return (_buffer[3] << 24) | (_buffer[2] << 16) | (_buffer[1] << 8) | (_buffer[0]);
    }

    template <class T>
    static void Print(T Input)
    {
        std::cout << Input << "\n";
    }
};

#endif