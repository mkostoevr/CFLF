# Cheat for "Let's Fish!"

## How to compile
### Simple version (relatively huge size, no icon)

    cd src
    gcc *.c -I../include -luser32 -lgdi32 -lkernel32 -DNO_MAKEFILE -o ../bin/CFLF.exe -mwindows

Result size for mingw-w64 i686-8.1.0-posix-dwarf-rt_v6-rev0: 67 045 Bytes

### Uberoptimized version

If you have installed:
* GNU Make
* GCC compatible C compiler
* [GoRc resource compiler](http://www.godevtool.com/#rc)
* [GoLink linker](http://www.godevtool.com/#linker)

Execute GNU Make with Makefile.
This will make small and fast executable file.

Result size for mingw-w64 i686-8.1.0-posix-dwarf-rt_v6-rev0: 23 040 Bytes
