using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Generic;

using LuaBackend.Libraries;
using MoonSharp.Interpreter;

namespace LuaBackend.Engine
{
    public class MainEngine
    {
        public List<Script> LoadedScripts;
        public Task RunTask;

        private bool _runBoolean = false;

        public MainEngine() => LoadedScripts = new List<Script>();

        public void InitializeScripts()
        {
            var _files = Directory.GetFiles("scripts", "*.lua");

            UserData.RegisterType<LibProcess>();
            UserData.RegisterType<LibConsole>();

            foreach (var _path in _files)
            {
                Console.WriteLine(String.Format("FOUND SCRIPT: {0}! Parsing...", _path));

                var _script = new Script();

                DynValue _libProcess = UserData.Create(new LibProcess());
                DynValue _libConsole = UserData.Create(new LibConsole());

                _script.Globals.Set("LibProcess", _libProcess);
                _script.Globals.Set("LibConsole", _libConsole);

                _script.DoFile(_path);

                var _initTask = _script.Globals.Get("_OnInit");
                var _frameTask = _script.Globals.Get("_OnFrame");

                if (_initTask.IsNil() && _frameTask.IsNil())
                {
                    Console.WriteLine(String.Format("ERROR: No event handlers can be found. Could not parse {0}!", _path));
                    continue;
                }

                if (_initTask.IsNil())
                    Console.WriteLine("WARNING: The event handler for initialization cannot be found.");

                if (_frameTask.IsNil())
                    Console.WriteLine("WARNING: The event handler for frame draw cannot be found.");

                LoadedScripts.Add(_script);

                Console.WriteLine(String.Format("MESSAGE: Parsed script {0} successfully!", _path));
            }
        }

        public void RunScripts()
        {
            Action _runFunction = delegate()
            {
                var _intreval = 0;

                while (_runBoolean)
                {
                    _intreval++;

                    if (_intreval >= 17)
                    {
                        foreach(var _script in LoadedScripts)
                        {
                            var _frameTask = _script.Globals.Get("_OnFrame").Function.GetDelegate();
                            _frameTask();
                        }

                        _intreval = 0;
                    }
                }
            };

            _runBoolean = true;

            RunTask = new Task(_runFunction);
            RunTask.Start();
        }
    
        public void StopScripts()
        {
            _runBoolean = false;
        }
    }
}
