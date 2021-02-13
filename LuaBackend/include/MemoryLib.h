#ifndef MEMORYLIB
#define MEMORYLIB

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

class MemoryLib
{
    public:
    static inline STARTUPINFOA _sInfo;
    static inline PROCESS_INFORMATION _pInfo;
    static inline uint32_t _baseAddress;
    static inline bool _bigEndian;

    static void ExecuteProcess(string InputName, uint32_t InputAddress, bool InputEndian)
    {
        ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
        ZeroMemory(&_pInfo, sizeof(_pInfo));

        CreateProcessA(InputName.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &_sInfo, &_pInfo);

        _baseAddress = InputAddress;
        _bigEndian = InputEndian;
    };

    static uint8_t ReadByte(int Address) { return ReadBytes(Address, 1)[0]; }
    static vector<uint8_t> ReadBytes(int Address, int Length)
    {
        vector<uint8_t> _buffer;
        _buffer.resize(Length);

        ReadProcessMemory(_pInfo.hProcess, (void*)(Address + _baseAddress), _buffer.data(), Length, 0);
        return _buffer;
    }
    static uint16_t ReadShort(int Address)
    {
        auto _buffer = ReadBytes(Address, 2);

        if (_bigEndian)
            return (_buffer[0] << 8) | _buffer[1];
        else
            return (_buffer[1] << 8) | _buffer[0];
    }
    static uint32_t ReadInt(int Address)
    {
        auto _buffer = ReadBytes(Address, 4);

        if (_bigEndian)
            return (_buffer[0] << 24) | (_buffer[1] << 16) | (_buffer[2] << 8) | (_buffer[3]);
        else
            return (_buffer[3] << 24) | (_buffer[2] << 16) | (_buffer[1] << 8) | (_buffer[0]);
    }
    static float ReadFloat(int Address)
    {
        auto _value = ReadInt(Address);
        auto _return = *reinterpret_cast<float*>(&_value);
        return _return;
    }
    static bool ReadBool(int Address) 
    {
        auto _value = ReadByte(Address);
        return _value == 0 ? false : true;
    }
    static string ReadString(int Address, int Length)
    {
        auto _value = ReadBytes(Address, Length);
        string _output(_value.begin(), _value.end());
        return _output;
    }

    static void WriteByte(int Address, uint8_t Input) { WriteProcessMemory(_pInfo.hProcess, (void*)(Address + _baseAddress), &Input, 1, 0); }
    static void WriteBytes(int Address, vector<uint8_t> Input) { WriteProcessMemory(_pInfo.hProcess, (void*)(Address + _baseAddress), Input.data(), Input.size(), 0); }
    static void WriteShort(int Address, uint16_t Input)
    {
        vector<uint8_t> _write(2);
        
        for (int i = 0; i < 2; i++)
        {
            if (_bigEndian)
                _write[1 - i] = (Input >> (i * 8)) & 0xFF;

            else
                _write[i] = (Input >> (i * 8)) & 0xFF;
        }

        WriteBytes(Address, _write);
        _write.clear();
    }
    static void WriteInt(int Address, uint32_t Input)
    {
        vector<uint8_t> _write(4);

        for (int i = 0; i < 4; i++)
        {
            if (_bigEndian)
                _write[3 - i] = (Input >> (i * 8)) & 0xFF;

            else
                _write[i] = (Input >> (i * 8)) & 0xFF;
        }

        WriteBytes(Address, _write);
        _write.clear();
    }
    static void WriteFloat(int Address, float Input)
    {
        auto _value = *reinterpret_cast<uint32_t*>(&Input);
        WriteInt(Address, _value);
    }
    static void WriteBool(int Address, bool Input) { Input == true ? WriteByte(Address, 1) : WriteByte(Address, 0); }
    static void WriteString(int Address, string Input)
    {
        vector<uint8_t> _value(Input.begin(), Input.end());
        WriteBytes(Address, _value);
    }
};
#endif