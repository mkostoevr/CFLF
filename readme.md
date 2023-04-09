# Cheat for "Let's Fish!"

## How to compile
### Simple version (relatively huge size, no icon)

    cd src
    gcc *.c -I../include  -ladvapi32 -luser32 -lgdi32 -lkernel32 -o ../bin/CFLF.exe -mwindows

Result size for mingw-w64 i686-8.1.0-posix-dwarf-rt_v6-rev0: 73 784 Bytes

### Uberoptimized version

If you have installed:
* GNU Make
* GCC compatible C compiler
* [GoRc resource compiler](http://www.godevtool.com/#rc)
* [GoLink linker](http://www.godevtool.com/#linker)

Execute GNU Make with Makefile.
This will make small and fast executable file.

Result size for mingw-w64 i686-8.1.0-posix-dwarf-rt_v6-rev0: 25 088 Bytes

Exact GoRc and GoLink I used may be found in GoTools package [here](http://magomed.org/file/CFLF_GoTools.zip).
