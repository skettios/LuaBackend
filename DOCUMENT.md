## LuaBackend Cheatsheet v2.7

---

### Global Variables

- ``GAME_ID`` => Returns the current game's CRC32 checksum as an integer. This checksum is generated from the process name at this moment.
- ``SCRIPT_PATH`` => Returns the path which Lua scripts are loaded from as a string. Ex: ``"C:\Users\Topaz\Desktop\LuaBackend\scripts"``
- ``CHEATS_PATH`` => Always returns ``"NOT_AVAILABLE"``
- ``ENGINE_VERSION`` => Returns the LuaBackend's engine version as a float. Ex: ``2.7``
- ``ENGINE_TYPE`` => Always returns ``"BACKEND"``

### Read\[XXXX\](Address)

Reads a certain value from **Address** calculated relative to the base address. Returns the read value. 
The "**XXXX**" is a placeholder for the value type.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    local _tempInt = ReadInt(0xDEADB00) -- Read an integer from address BaseAddress+0xDEADB00
    local _tempByte = ReadByte(0xB007555) -- Read a byte from address BaseAddress+0xB007555
```

### Write\[XXXX\](Address, Value)

Writes **Value** to **Address** calculated relative to the base address. 
The "**\[XXXX\]**" is a placeholder for the value type.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    WriteShort(0xDEADB00, 0xFFFF) -- Write 0xFFFF to BaseAddress+0xDEADB00
    WriteBoolean(0xB007555, true) -- Write true to BaseAddress+0xB007555
```

###  Read\[XXXX\]A(Address)

Reads a certain value from absolute **Address**. Returns the read value.  
You generally only need this for absolute operations. Most of the time, you should not use this.  
The "**\[XXXX\]**" is a placeholder for the value type.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    local _tempInt = ReadIntA(0xB00BA) -- Read an integer from address 0xB00BA
    local _tempByte = ReadByteA(0xEEEEEE) -- Read a byte from address 0xEEEEEE
```

### Write\[XXXX\]A(Address, Value)

Writes **Value** to absolute **Address**.  
You generally only need this for absolute operations. Most of the time, you should not use this.  
The "**\[XXXX\]**" is a placeholder for the value type.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    WriteShortA(0xBABEEEE, 0xFFFF) -- Write 0xFFFF to 0xBABEEEE
    WriteBooleanA(0x5355355, true) -- Write true to 0x5355355
```

### ReadArray(Address, Length)

Reads **Length** amount of bytes from memory, starting at **Address**. Returns the read bytes as an array.  

Example:
```lua
    local _tempArray = ReadArray(0xBEEFDED, 0x10) -- Reads 16 bytes starting at BaseAddress+0xBEEFDED
```

### WriteArray(Address, Array)

Writes **Array** to the memory, starting at **Address**. The array must be made of bytes (8-bit integers).  
Example:
```lua
    local _tempArray = { 0xFE, 0xA5, 0x70 }
    WriteArray(0x6660420, _tempArray) -- Writes _tempArray to memory, starting at BaseAddress+0x6660420
```

### ReadArrayA(Address, Length)

Reads **Length** amount of bytes from memory, starting at absolute **Address**. Returns the read bytes as an array.  
You generally only need this for absolute operations. Most of the time, you should not use this. 

Example:
```lua
    local _tempArray = ReadArrayA(0xBEEFDED, 0x10) -- Reads 16 bytes starting at 0xBEEFDED
```

### WriteArrayA(Address, Array)

Writes **Array** to the memory, starting at absolute **Address**. The array must be made of bytes (8-bit integers).  
You generally only need this for absolute operations. Most of the time, you should not use this. 

Example:
```lua
    local _tempArray = { 0xFE, 0xA5, 0x70 }
    WriteArrayA(0x6660420, _tempArray) -- Writes _tempArray to memory, starting at 0x6660420
```

### GetPointer(Address, Offset)

Calculates a pointer with the given values.  
Basically, reads the value at **Address** and adds **Offset** to it. Returns the resulting value.

Example:
```lua
    local _tempPointer = GetPointer(0xBEEFDAD, 0x15) -- Reads the value at BaseAddress+0xBEEFDAD and adds 0x15 to it.
```

### GetPointerA(Address, Offset)

Calculates a pointer with the given values.  
Basically, reads the value at absolute **Address** and adds **Offset** to it. Returns the resulting value.  
You generally only need this for absolute operations. Most of the time, you should not use this. 

Example:
```lua
    local _tempPointer = GetPointerA(0xBEEFDAD, 0x15) -- Reads the value at 0xBEEFDAD and adds 0x15 to it.
```

### WriteExec(Address, Value) - [LuaBackend Only]

Writes the given byte array **Value** to **Address** relative to the current EXE's address, NOT the provided base address.

Example:
```lua
    WriteExec(0x00FBAAD, 0x15) -- Writes 0x15 to EXE_NAME.exe+0x00FBAAD.

```

### ConsolePrint(Message)

Prints unto the console. Compatible with LuaEngine's implementation, but lacking in color.

Example:
```lua
    ConsolePrint("Howdy partner?") -- Prints to the Console: "Howdy partner?".
```

#### * Any functions present in this document signify compatibility with the latest version. Any function absent in this document is no longer supported and should be considered obsolete.
