O=-Ofast -march=i686 -mtune=i686 -s
LANG=-DLANG_EN # EN, RU
W=-Wall -Wextra -Wpedantic
C=i686-w64-mingw32-gcc
L=golink # just cause this fucking shitty-ported ld sucks my dick
R=gorc # shit, golink not works with windres output...
CFLAGS=-Iinclude -DMAKEFILE -DONE_SOURCE $(LANG) $(O) $(W) -nostdlib -ffreestanding -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -fno-leading-underscore -fno-ident -fno-asynchronous-unwind-tables -c -o # here is output file name
LFLAGS=kernel32.dll user32.dll gdi32.dll msvcrt.dll /mix /entry Start /fo # here is output file name... and this stupid linker does not accept "main" entry... nothing working properly on this damn operating system
RFLAGS=/r /fo # here is output file name
DEL=del /q # here is deleting file name/pattern

SRC=src/cflf.c
OBJ=obj/CFLF.obj
RC=cflf.rc
RES=obj/CFLF.res
EXE=bin/CFLF.exe

all: $(EXE)
	./$(EXE)

$(EXE): $(OBJ) $(RES)
	$(L) $(LFLAGS) $(EXE) $(OBJ) $(RES)

$(OBJ): src/*.c
	$(C) $(CFLAGS) $(OBJ) $(SRC)

$(RES): $(RC)
	$(R) $(RFLAGS) $(RES) $(RC)

clean:
	$(DEL) bin\*
	$(DEL) obj\*
