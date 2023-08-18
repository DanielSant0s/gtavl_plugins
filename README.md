# GTA V Legacy Plugin SDK
 A plugin loader and software development kit for GTA V Legacy.

## How to use
Run VLSDK.bat located at msys/1.0 and it will open a terminal window, type "cd gtavl" and you are on the project folder, located at msys/1.0/home/gtavl

### Working with the project
* make - Builds the project and generates gtavl.erl, which will be loaded inside ISO root folder.
* make clean - Cleans all temporary files.  

All the project still being written in C and it has some file/folder patterns. plugin_ps2 has PS2 system internal functions and structs, plugin_sa has GTA San Andreas internal variables, consts, structs, functions, classes and methods, src is GTA V Legacy source code itself. All plugin_* stuff lacks a lot of functions so you can contribute for the project documenting structs and functions using IDA Pro comparisions and Plugin SDK (for Windows) implements.

Links: (TODO: ADD LINKS)

* GTA V Legacy latest ISO file
* GTA San Andreas PS2 Database (IDA Pro)
* GTA San Andreas PC Database (IDA Pro)
* IDA Pro 7.5
