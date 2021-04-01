# LuaBackend
![Windows Build](https://github.com/Topaz-Reality/LuaBackend/workflows/Windows%20Build/badge.svg)

---

## What is LuaBackend?

LuaBackend is a standalone script interpreter for PC games. It attaches itself to the PC application and executes Lua scripts.

## How do I use LuaBackend mod that I have downloaded?

To use a LuaBackend script, all you have to do is to drop the script you downloaded inside the "**scripts**" folder.
However, LuaBackend also has I/O capability. So if a mod you have downloaded also has "**io_load**" or "**io_packages**" directories included, 
you would need to drop those folders to that "**scrripts**" folder as well.

## How do I use LuaEngine to create a mod myself?

Create a new lua file inside of the **scripts** folder. Any number of scripts and any name is fine.
Inside the file, copy and paste this snippet:

```lua
function _OnInit()
   -- Code here executes on script load.
end

function _OnFrame()
   -- Code here executes on every frame.
end
```

The script in question should have at least one of these functions to work. The "**_OnInit**" function is executed when the script is first loaded, 
and the "**_OnFrame**" function is executed every frame. So put the things you want to happen every frame into the "**_OnFrame**" function.

That is the minimum you would need to have a functioning script. The rest is on you. The scripts in LuaEngine function just as if they were regular Lua scripts.
Because... they kind of are! Below you can find the cheatsheet to all LuaBackend specific functions.

## How do I use libraries made for Lua with LuaBackend?

The libraries you would need to use must be 5.4 compliant. In addition, if the libraries need to be compiled, they must be compiled
for x80_64 and with Lua 5.4 with the MSVC 2019 compiler.

Put the libraries on the "**io_packages**" folder in the "**lua**" folder. To reference them, you can then use the ``require`` command.
Exampe being, if I want to use ``socket.http`` from LuaSocket, which I put in ``scripts/io_packages/LuaSocket/``, I would need to do:

```lua
local http = require("LuaSocket/socket.http")
```

That's it!

## Important Notes to using LuaBackend

- A config file for the game you want to mod must be made. More on that below.
- All values are unsigned.
- There is no limit for the amount of scripts loaded at this moment.

## LuaBackend "config.ini" Template

```ini
  [CONFIG]
  EXECUTABLE = KINGDOM HEARTS II FINAL MIX.exe
  BASE_ADDRESS = KINGDOM HEARTS II FINAL MIX.exe+56450E
  BIG_ENDIAN = false
  ATTACH = true
```

- ``EXECUTABLE`` => Name of the executable you want to mod.
- ``BASE_ADDRESS`` => The base address of the application. Can be static (0x20000000) or can be module's compliment (KINGDOM HEARTS II FINAL MIX.exe+56450E).
- ``ATTACH`` => Optional. If set to 'true', it will attach to an already running EXE. If not set or 'false', it will launch said executable and attach. If this value is not set or is 'false', LuaBackend must be at the same location as the given EXE.
- ``BIG_ENDIAN`` => Optional. Data will be written in Big Endian if set tu 'true'. Required for RPCS3.

## LuaBackend Cheatsheet

---

### Global Variables

- ``GAME_ID`` => Returns the current game's CRC32 checksum as an integer. Ex: ``0xF266B00B``
- ``SCRIPT_PATH`` => Returns the path which Lua scripts are loaded from as a string. Ex: ``"C:\Users\Topaz\Desktop\LuaBackend\scripts"``
- ``CHEATS_PATH`` => Always returns ``"NOT_AVAILABLE"``
- ``ENGINE_VERSION`` => Returns the LuaBackend's engine version as a float. Ex: ``2.5``
- ``ENGINE_TYPE`` => Always returns ``"BACKEND"``


###  ReadXXXX(Address)

Reads a certain value from **Address**. Returns the read value. 
The "**XXXX**" is a placeholder for the value type.
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    local _tempInt = ReadInt(0xDEADB00) -- Read an integer from address 0xDEADB00
    local _tempByte = ReadByte(0xB007555) -- Read a byte from address 0xB007555
```

### WriteXXXX(Address, Value)

Writes **Value** to **Address**. 
The "**XXXX**" is a placeholder for the value type.
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    WriteShort(0xDEADB00, 0xFFFF) -- Write 0xFFFF to 0xDEADB00
    WriteBoolean(0xB007555, true) -- Write true to 0xB007555
```

### ReadArray(Address, Length)

Reads **Length** amount of bytes from memory, starting at **Address**. Returns the read bytes as an array.

Example:
```lua
    local _tempArray = ReadArray(0xBEEFDED, 0x10) -- Reads 16 bytes starting at 0xBEEFDED
```

### WriteArray(Address, Array)

Writes **Array** to the memory, starting at **Address**. The array must be made of bytes (8-bit integers).

Example:
```lua
    local _tempArray = { 0xFE, 0xA5, 0x70 }
    WriteArray(0x6660420, _tempArray) -- Writes _tempArray to memory, starting at 0x6660420
```

### ReadFile(FileName)

Reads the file "**lua/io_load/FileName**" to an array.

Example:
```lua
    local _tempFile = ReadFile("killMePl0x.kek") -- Reads the file at lua/io_load/killMePl0x.kek to _tempFile. Re-reading the file is unnecessary from this point.
```
### ReadFile(FileName, Offset, Size)

Reads the **Size** bytes from "**lua/io_load/FileName**" to an array, starting from **Offset** .

Example:
```lua
    local _tempFile = ReadFile("killMePl0x.kek", 0x0069, 0x0420) -- Reads 0x0420 bytes from the file at lua/io_load/killMePl0x.kek to _tempFile, starting from 0x0069
```
### GetPointer(Address, Offset)

Calculates a pointer with the given values. Basically, reads the value at **Address** and adds **Offset** to it. Returns the resulting value.

Example:
```lua
    local _tempPointer = GetPointer(0xBEEFDAD, 0x15) -- Reads the value at 0xBEEFDAD and adds 0x15 to it.
```

### WriteExec(Address, Value)

Writes the given byte array **Value** to **Address** relative to the current EXE's address, NOT the provided base address.
Example:
```lua
    WriteExec(0x00FBAAD, 0x15) -- Writes 0x15 to EXE_NAME.exe+0x00FBAAD.
```

### ConsolePrint(Message, UNUSED)

Prints unto the console. Compatible with LuaEngine's implementation, but lacking in color.

Example:
```lua
    ConsolePrint("Howdy partner?") -- Prints to the Console: "Howdy partner?".
```

#### * Any functions present in this document signify compatibility with the latest version. Any function absent in this document is no longer supported and should be considered obsolete.
