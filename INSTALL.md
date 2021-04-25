## Installing LuaBackend

There are two versions of LuaBackend that you can install. Please follow this document for the specific version you want to use.

### External LuaBackend [EXE]
- Download ``LuaBackend.7z`` from the Releases tab.
- Extract the contents of ``LuaBackend.7z/External`` to a folder.
- If the game you want to use LuaBackend with has a preset config, find it in the ``preset_configs`` folder of the folder you extracted. Copy it to be side by side with ``LuaBackend.exe`` and rename it to ``config.ini``
- Otherwise, create a new file called ``config.ini`` and see [the config creation section.](#luabackend-config-creation)
- Put the scripts you want to use to the ``scripts`` folder of the folder you extracted.
- Launch LuaBackend and do not launch the game. If you get a ``Link Failed`` error, launch LuaBackend **after** launching the game.

### Internal LuaBackend [DLL]
- Download ``LuaBackend.7z`` from the Releases tab.
- Extract the contents of ``LuaBackend.7z/Internal`` to a folder.
- Copy both the DLLs inside the folder to your game's EXE location.
- If your game is supported, it will have a patch file for it in the ``patches`` folder of the folder you extracted. Patch your game's EXE with it using [this tool](https://www.marcrobledo.com/RomPatcher.js/) and replace that EXE with the patched version. (You need to do this only once, unless told otherwise)
- At this point, LuaBackend is installed internally. Launch the game. The location you need to put your script files into will be told to you on launch if the location does not exist (So, ideally, only once.)

## LuaBackend Config Creation

Copy and paste the template below:
```ini
[CONFIG]
EXECUTABLE = EXE_NAME
BASE_ADDRESS = EXE_ADDRESS
BIG_ENDIAN = false
ATTACH = false
```
Now, edit the variables according to the below format:

- ``EXECUTABLE`` => Name of the executable you want to mod.
- ``BASE_ADDRESS`` => The base address of the application. Can be static (0x20000000) or can be module's compliment (KINGDOM HEARTS II FINAL MIX.exe+56450E).
- ``ATTACH`` => Optional, required for games that require a launcher. If set to ``true``, it will attach to an already running EXE. If not set or set to ``false``, it will launch said executable and attach.  
  If this value is not set or is 'false', LuaBackend must be at the same location as the given EXE.
- ``BIG_ENDIAN`` => Optional. Data will be written in Big Endian if set tu 'true'. Required for emulators like RPCS3, should not be used with emulators like PPSSPP.
