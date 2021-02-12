using System;
using System.Collections.Generic;
using System.Text;

using LuaBackend.Backend;
using MoonSharp.Interpreter;

namespace LuaBackend.Libraries
{
    class LibProcess
    {
        public byte ReadByte(int Address) => LibMemory.ReadByte(Address + LibMemory.GameAddress);
        public ushort ReadShort(int Address) => LibMemory.ReadShort(Address + LibMemory.GameAddress);
        public uint ReadInt(int Address) => LibMemory.ReadInt(Address + LibMemory.GameAddress);
        public float ReadFloat(int Address) => LibMemory.ReadFloat(Address + LibMemory.GameAddress);
        public bool ReadBoolean(int Address) => LibMemory.ReadBool(Address + LibMemory.GameAddress);
        public string ReadString(int Address, int Length) => LibMemory.ReadString(Address + LibMemory.GameAddress, Length);
        public byte[] ReadArray(int Address, int Length) => LibMemory.ReadBytes(Address + LibMemory.GameAddress, Length);
    }
}
