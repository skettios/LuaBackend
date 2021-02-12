using System;

using LuaBackend.Backend;
using LuaBackend.Engine;

namespace LuaBackend
{
    class Program
    {
        static MainEngine _engine;

        static void Main(string[] args)
        {
            LibMemory.LaunchProcess("emu/pcsx2.exe");
            
            _engine = new MainEngine();
            _engine.InitializeScripts();
            _engine.RunScripts();

            Console.ReadKey();
        }
    }
}
