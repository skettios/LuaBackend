// LOCATION: EXE -> AxaFormBase -> BaseSimpleForm
// STARTING: N/A

using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;

public static CancellationTokenSource CancelSource;
public static CancellationToken LuaToken;
public static Task LuaTask;

[DllImport("LuaBackend.dll", CallingConvention = CallingConvention.Cdecl)]
public static extern int EntryLUA(int InputID, IntPtr InputHandle, ulong InputAddress, string InputPath);

[DllImport("LuaBackend.dll", CallingConvention = CallingConvention.Cdecl)]
public static extern int ExecuteLUA();

[DllImport("LuaBackend.dll", CallingConvention = CallingConvention.Cdecl)]
public static extern bool CheckLUA();

[DllImport("LuaBackend.dll", CallingConvention = CallingConvention.Cdecl)]
public static extern int VersionLUA();

// LOCATION: EXE -> AxaFormBase -> BaseSimpleForm.createInstance()
// STARTING: Line #09

string _scrFolder = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Personal) + "\\KINGDOM HEARTS HD 1.5+2.5 ReMIX\\scripts";

CancelSource = new CancellationTokenSource();
LuaToken = CancelSource.Token; 

if (File.Exists("LuaBackend.dll"))
{
    if (!Directory.Exists(_scrFolder))
        MessageBox.Show("The \"" + _scrFolder + "\" folder was not found.\nLuaEngine will not operate.", 
                        "KINGDOM HEARTS Birth by Sleep FINAL MIX", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

    else
    {
        if (VersionLUA() > 128)
            MessageBox.Show("The LuaEngine DLL is too new for this executable.\nLuaEngine will not operate until this EXE is updated.", 
                            "KINGDOM HEARTS Birth by Sleep FINAL MIX", MessageBoxButtons.OK, MessageBoxIcon.Error);

        else if (VersionLUA() < 128)
            MessageBox.Show("The LuaEngine DLL is too old for this executable.\nLuaEngine will not operate until the DLL is updated.", 
                "KINGDOM HEARTS Birth by Sleep FINAL MIX", MessageBoxButtons.OK, MessageBoxIcon.Error);

        else
        {
            long _addr = Process.GetCurrentProcess().MainModule.BaseAddress.ToInt64() + 0x60E334;
            
            LuaTask = Task.Factory.StartNew(delegate()
            {
                if (EntryLUA(Process.GetCurrentProcess().Id, Process.GetCurrentProcess().Handle, (ulong)_addr, _scrFolder) == 0)
                {
                    while (!LuaToken.IsCancellationRequested)
                    {
                        if (!CheckLUA())
                        {
                            Console.Clear();
                            EntryLUA(Process.GetCurrentProcess().Id, Process.GetCurrentProcess().Handle, (ulong)_addr, _scrFolder);
                        }

                        else
                            ExecuteLUA();
                    }
                }
            }, LuaToken);
        }
    }
}

// LOCATION: EXE -> AxaFormBase -> BaseSimpleForm.Dispose()
// STARTING: Line #06

CancelSource.Cancel();
