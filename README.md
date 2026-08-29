# WheelmanML - a Mod Loader for Wheelman
> This is still a very early work-in-progress concept, but I hope that this can one day open a door to modding Wheelman.

## Compiling
The path to the Developer Command Prompt from Visual Studio in `build.bat` needs to be changed to the correct path of your installation in order to compile the DLL.
After compiling, the final `xinput9_1_0.dll` will be put in the folder `out`.

## Usage
Place the files `xinput9_1_0.dll` and `xinput9_1_0_original.dll` in the `Binaries` folder of Wheelman, or rename the original `xinput9_1_0.dll` to `xinput9_1_0_original.dll` before placing the modified DLL there.

The `Binaries` folder is usually located at `C:\Program Files (x86)\Ubisoft Entertainment\Wheelman\Binaries\`.

This is also where the mod loader looks for and loads `.ASI` files from.