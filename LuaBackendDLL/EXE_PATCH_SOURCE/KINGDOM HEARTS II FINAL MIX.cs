// Placed inside EXE -> AxaFormBase -> BaseSimpleForm.createInstance()

string _scrFolder = Environment.GetFolderPath(Environment.SpecialFolder.Personal) + "\\KINGDOM HEARTS HD 1.5+2.5 ReMIX\\scripts";

BaseSimpleForm.CancelSource = new CancellationTokenSource();
BaseSimpleForm.LuaToken = BaseSimpleForm.CancelSource.Token; 

if (File.Exists("LuaBackend.dll"))
{
    if (!Directory.Exists(_scrFolder))
        MessageBox.Show("The \"" + _scrFolder + "\" folder was not found.\nLuaEngine will not operate.", "KINGDOM HEARTS II FINAL MIX", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

    else
    {
        long _addr = Process.GetCurrentProcess().MainModule.BaseAddress.ToInt64() + 0x56450E;
        
        BaseSimpleForm.LuaTask = Task.Factory.StartNew(delegate()
        {
            if (BaseSimpleForm.EntryLUA(Process.GetCurrentProcess().Id, Process.GetCurrentProcess().Handle, (ulong)_addr, _scrFolder) == 0)
            {
                while (!BaseSimpleForm.LuaToken.IsCancellationRequested)
                {
                    BaseSimpleForm.ExecuteLUA();
                }
            }
        }, BaseSimpleForm.LuaToken);
    }
}