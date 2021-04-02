# LuaBackend
![Windows Build](https://github.com/Topaz-Reality/LuaBackend/workflows/Windows%20Build/badge.svg)

## What is LuaBackend?

LuaBackend is a standalone script interpreter for PC games. It attaches itself to the PC application and executes Lua scripts.    

There are two different LuaBackend versions:
 - LuaBackendEXE: External, attaches to any process.
 - LuaBackendDLL: Internal, accompanied by an EXE Patch, does not require configuration.

## How do I use LuaBackend mod that I have downloaded?

To use a LuaBackend script, all you have to do is to drop the script you downloaded inside the "**scripts**" folder. For the external version of LuaBackend, it is in the executable folder. For the internal version, this folder's location varies and will be told to you by the game in question if now found.  

However, LuaBackend also has I/O capability. So if a mod you have downloaded also has "**io_load**" or "**io_packages**" directories included, 
you would need to drop those folders to that "**scripts**" folder as well.

## How do I use libraries made for Lua with LuaBackend?

The libraries you would need to use must be 5.4 compliant. In addition, if the libraries need to be compiled, they must be compiled
for x80_64 and with Lua 5.4 with the MSVC 2019 compiler.

Put the libraries on the "**io_packages**" folder in the "**scripts**" folder. To reference them, you can then use the ``require`` command.
Exampe being, if I want to use ``socket.http`` from LuaSocket, which I put in ``scripts/io_packages/LuaSocket/``, I would need to do:

```lua
local http = require("LuaSocket/socket.http")
```

That's it!

<<<<<<< HEAD
## How do I use LuaBackend to create a mod myself?
=======
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

## LuaBackend Preset Configs

In your downloaded ZIP, there will be configs already made for the games you want to use them with. Simply drag one of them out so it's next to the LuaBackend EXE and rename it to ``"config.ini"``.

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
>>>>>>> 18e102c2c2a74def50bebda10b7959aed629d31e

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
Because... they kind of are!  

In DOCUMENT.md, you can find the cheatsheet to all LuaBackend specific functions.  
In INSTALL.md, you can find how to install each version of LuaBackend.

## Important Notes to using LuaBackend
- All values are unsigned.
- There is no limit for the amount of scripts loaded at this moment.