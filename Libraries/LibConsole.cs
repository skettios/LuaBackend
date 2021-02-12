using System;
using System.Collections.Generic;
using System.Text;

namespace LuaBackend.Libraries
{
    class LibConsole
    {
        public void Print(string Message)
        {
            var _output = string.Format("> {0}", Message);
            Console.WriteLine(_output);
        }

        public void Print(string Message, int Color)
        {
            var _output = string.Format("> {0}", Message);
            var _color = (ConsoleColor)Color;

            Console.ForegroundColor = _color;
            Console.WriteLine(_output);

            Console.ResetColor();
        }
    }
}
