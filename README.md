# LuaBackend
![Windows Build](https://github.com/Topaz-Reality/LuaBackend/workflows/Windows%20Build/badge.svg)

## What is LuaBackend?

LuaBackend is a standalone script interpreter for PC games. It attaches itself to the PC application and executes Lua scripts. 
It is 95% compatible with it's sister, LuaEngine for PCSX2-EX.   

There are two different LuaBackend versions:
 - LuaBackendEXE: External, attaches to any process.
 - LuaBackendDLL: Internal, accompanied by an EXE Patch, does not require configuration.

## How do I use LuaBackend mod that I have downloaded?

To use a LuaBackend script, all you have to do is to drop the script you downloaded inside the "**scripts**" folder. For the external version of LuaBackend, it is in the executable folder. For the internal version, this folder's location varies and will be told to you by the game in question if now found.  

However, LuaBackend also has I/O capability. So if a mod you have downloaded also has "**io_load**" or "**io_packages**" directories included, 
you would need to drop those folders to that "**scripts**" folder as well.

## How do I use libraries made for Lua with LuaBackend?

The libraries you would need to use must be 5.4 compliant. In addition, if the libraries need to be compiled, they must be compiled
for x86_64 and with Lua 5.4 with the MSVC 2019 compiler.

Put the libraries on the "**io_packages**" folder in the "**scripts**" folder. To reference them, you can then use the ``require`` command.
Exampe being, if I want to use ``socket.http`` from LuaSocket, which I put in ``scripts/io_packages/LuaSocket/``, I would need to do:

```lua
local http = require("LuaSocket/socket.http")
```

That's it!

## How do I use LuaBackend to create a mod myself?

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

## External Libraries

http://www.lua.org/  
https://github.com/pulzed/mINI  
https://github.com/ThePhD/sol2  
https://github.com/d-bahr/CRCpp  
https://github.com/discord/discord-rpc  
