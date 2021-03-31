#ifndef MEMORYLIB
#define MEMORYLIB

#include <windows.h>
#include <iostream>
#include <string>
#include "TlHelp32.h"
#include <psapi.h>

using namespace std;

class MemoryLib
{
    public:
    static inline STARTUPINFOA _sInfo;
    static inline PROCESS_INFORMATION _pInfo;

    static inline DWORD _pIdentifier = NULL;
    static inline HANDLE _pHandle = NULL;

    static inline uint64_t _baseAddress;
    static inline uint64_t _execAddress;

    static inline bool _bigEndian;

    static HMODULE FindBaseAddr(HANDLE InputHandle, string InputName)
    {
        HMODULE hMods[1024];
        DWORD cbNeeded;
        unsigned int i;

        if (EnumProcessModules(InputHandle, hMods, sizeof(hMods), &cbNeeded))
        {
            for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(InputHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    wstring wstrModName = szModName;
                    wstring wstrModContain = getWideStr(InputName);

                    if (wstrModName.find(wstrModContain) != string::npos)
                        return hMods[i];
                }
            }
        }

        return nullptr;
    }

    static wstring getWideStr(const std::string& s)
    {
        std::wstring _wideTemp(s.begin(), s.end());
        return _wideTemp;
    }

    static DWORD FindProcessId(const std::wstring& processName)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (processesSnapshot == INVALID_HANDLE_VALUE)
            return 0;

        Process32First(processesSnapshot, &processInfo);
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }

        while (Process32Next(processesSnapshot, &processInfo))
        {
            if (!processName.compare(processInfo.szExeFile))
            {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }
        }

        CloseHandle(processesSnapshot);
        return 0;
    }

    static void SetExecAddr(uint64_t InputAddress)
    {
        _execAddress = InputAddress;
    }

    static void SetBaseAddr(uint64_t InputAddress)
    {
        _baseAddress = InputAddress;
    }

    static void ExecuteProcess(string InputName, uint64_t InputAddress, bool InputEndian)
    {
        ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
        ZeroMemory(&_pInfo, sizeof(_pInfo));

        CreateProcessA(InputName.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &_sInfo, &_pInfo);

        _baseAddress = InputAddress;
        _bigEndian = InputEndian;
    };

    static bool LatchProcess(string InputName, uint64_t InputAddress, bool InputEndian)
    {
        ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
        ZeroMemory(&_pInfo, sizeof(_pInfo));

        _pIdentifier = FindProcessId(getWideStr(InputName));
        _pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, _pIdentifier);

        _baseAddress = InputAddress;
        _bigEndian = InputEndian;

        if (_pHandle == NULL)
            return false;

        return true;
    };

    static uint8_t ReadByte(uint64_t Address) { return ReadBytes(Address, 1)[0]; }
    static vector<uint8_t> ReadBytes(uint64_t Address, int Length)
    {
        vector<uint8_t> _buffer;
        _buffer.resize(Length);

        ReadProcessMemory(_pHandle, (void*)(Address + _baseAddress), _buffer.data(), Length, 0);
        return _buffer;
    }
    static uint16_t ReadShort(uint64_t Address)
    {
        auto _buffer = ReadBytes(Address, 2);

        if (_bigEndian)
            return (_buffer[0] << 8) | _buffer[1];
        else
            return (_buffer[1] << 8) | _buffer[0];
    }
    static uint32_t ReadInt(uint64_t Address)
    {
        auto _buffer = ReadBytes(Address, 4);

        if (_bigEndian)
            return (_buffer[0] << 24) | (_buffer[1] << 16) | (_buffer[2] << 8) | (_buffer[3]);
        else
            return (_buffer[3] << 24) | (_buffer[2] << 16) | (_buffer[1] << 8) | (_buffer[0]);
    }
    static float ReadFloat(uint64_t Address)
    {
        auto _value = ReadInt(Address);
        auto _return = *reinterpret_cast<float*>(&_value);
        return _return;
    }
    static bool ReadBool(uint64_t Address) 
    {
        auto _value = ReadByte(Address);
        return _value == 0 ? false : true;
    }
    static string ReadString(uint64_t Address, int Length)
    {
        auto _value = ReadBytes(Address, Length);
        string _output(_value.begin(), _value.end());
        return _output;
    }

    static void WriteByte(uint64_t Address, uint8_t Input) { WriteProcessMemory(_pHandle, (void*)(Address + _baseAddress), &Input, 1, 0); }
    static void WriteBytes(uint64_t Address, vector<uint8_t> Input) 
    { 
        WriteProcessMemory(_pHandle, (void*)(Address + _baseAddress), Input.data(), Input.size(), 0); 
    }
    static void WriteShort(uint64_t Address, uint16_t Input)
    {
        vector<uint8_t> _write(2);
        
        for (uint64_t i = 0; i < 2; i++)
        {
            if (_bigEndian)
                _write[1 - i] = (Input >> (i * 8)) & 0xFF;

            else
                _write[i] = (Input >> (i * 8)) & 0xFF;
        }

        WriteBytes(Address, _write);
        _write.clear();
    }
    static void WriteInt(uint64_t Address, uint32_t Input)
    {
        vector<uint8_t> _write(4);

        for (uint64_t i = 0; i < 4; i++)
        {
            if (_bigEndian)
                _write[3 - i] = (Input >> (i * 8)) & 0xFF;

            else
                _write[i] = (Input >> (i * 8)) & 0xFF;
        }

        WriteBytes(Address, _write);
        _write.clear();
    }
    static void WriteFloat(uint64_t Address, float Input)
    {
        auto _value = *reinterpret_cast<uint32_t*>(&Input);
        WriteInt(Address, _value);
    }
    static void WriteBool(uint64_t Address, bool Input) { Input == true ? WriteByte(Address, 1) : WriteByte(Address, 0); }
    static void WriteString(uint64_t Address, string Input)
    {
        vector<uint8_t> _value(Input.begin(), Input.end());
        WriteBytes(Address, _value);
    }

    static void WriteExec(uint64_t Address, vector<uint8_t> Input)
    {
        WriteProcessMemory(_pHandle, (void*)(Address + _execAddress), Input.data(), Input.size(), 0);
    }
};
#endif