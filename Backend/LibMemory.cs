using System;
using System.Linq;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace LuaBackend.Backend
{
    public static class LibMemory
    {
        private const int PROCESS_ALL_ACCESS = 0x001F0FFF;

        private static Process _pLaunch;
        private static IntPtr _pHandle;

        public static IntPtr BaseAddress = IntPtr.Zero;
        public static int GameAddress = 0x20000000;

        #region DLL Imports
        [DllImport("kernel32.dll")]
        public static extern IntPtr OpenProcess(int dwDesiredAccess, bool bInheritHandle, int dwProcessId);
       
        [DllImport("kernel32", EntryPoint = "ReadProcessMemory", ExactSpelling = true, SetLastError = true)]
        private static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, out int lpNumberOfBytesRead);
        
        [DllImport("kernel32.dll")]
        public static extern Int32 CloseHandle(IntPtr hProcess);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int nSize, [Out] int lpNumberOfBytesWritten);
        #endregion

        /// <summary>
        /// Launches a process and creates a hook unto it.
        /// </summary>
        /// <param name="ProcessPath">Path of the executable.</param>
        /// <returns></returns>
        public static bool LaunchProcess(string ProcessPath)
        {
            _pLaunch = Process.Start(ProcessPath);

            if (_pLaunch != null)
            {
                _pHandle = OpenProcess(PROCESS_ALL_ACCESS, true, _pLaunch.Id);

                if (_pHandle != null)
                {
                    BaseAddress = _pLaunch.MainModule.BaseAddress;
                    return true;
                }

                else
                    return false;
            }

            else
                return false;
        }

        /// <summary>
        /// Reads a number of bytes from a specific part of the memory,
        /// and returns the read value as an array.
        /// </summary>
        /// <param name="Address">Starting point of the array.</param>
        /// <param name="Length">Length of the array.</param>
        /// <returns></returns>
        public static byte[] ReadBytes(int Address, int Length)
        {
            var _buffer = new byte[Length];
            var _read = 0;

            ReadProcessMemory(_pHandle, (IntPtr)Address, _buffer, Length, out _read);
            return _buffer;
        }

        /// <summary>
        /// Reads a single byte from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the byte in memory.</param>
        /// <returns></returns>
        public static byte ReadByte(int Address)
        {
            var _readValue = ReadBytes(Address, 1);
            return _readValue[0];
        }

        /// <summary>
        /// Reads a 16-bit value from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the value in memory.</param>
        /// <returns></returns>
        public static ushort ReadShort(int Address)
        {
            var _readValue = ReadBytes(Address, 4);
            var _returnValue = BitConverter.ToUInt16(_readValue, 0);
            return _returnValue;
        }

        /// <summary>
        /// Reads a 32-bit value from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the value in memory.</param>
        /// <returns></returns>
        public static uint ReadInt(int Address)
        {
            var _readValue = ReadBytes(Address, 4);
            var _returnValue = BitConverter.ToUInt32(_readValue, 0);
            return _returnValue;
        }
        
        /// <summary>
        /// Reads a 64-bit value from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the value in memory.</param>
        /// <returns></returns>
        public static ulong ReadLong(int Address)
        {
            var _readValue = ReadBytes(Address, 8);
            var _returnValue = BitConverter.ToUInt64(_readValue, 0);
            return _returnValue;
        }

        /// <summary>
        /// Reads a single precision floating point from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the value in memory.</param>
        /// <returns></returns>
        public static float ReadFloat(int Address)
        {
            var _readValue = ReadBytes(Address, 4);
            var _returnValue = BitConverter.ToSingle(_readValue, 0);
            return _returnValue;
        }

        /// <summary>
        /// Reads a boolean from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the value in memory.</param>
        /// <returns></returns>
        public static bool ReadBool(int Address)
        {
            var _readValue = ReadBytes(Address, 1);
            var _returnValue = _readValue[0] == 1 ? true : false;
            return _returnValue;
        }

        /// <summary>
        /// Reads a string with a specific length from the memory and returns it.
        /// </summary>
        /// <param name="Address">Location of the string in memory.</param>
        /// <param name="Length">Length of the string in memory.</param>
        /// <returns></returns>
        public static string ReadString(int Address, int Length)
        {
            var _readValue = ReadBytes(Address, Length);
            var _returnValue = Encoding.ASCII.GetString(_readValue);

            return _returnValue;
        }
    }
}
