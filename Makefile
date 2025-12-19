CC = gcc
.PHONY: all clean

all: impl_a.dll impl_b.dll program1.exe program2.exe #при make строит эти файлы

impl_a.dll: impl_a.c contract.h
	$(CC) -DBUILD_DLL -shared -o impl_a.dll impl_a.c -Wl,--out-implib,libimpl_a.a

#импортная либа не нужна
impl_b.dll: impl_b.c contract.h
	$(CC) -DBUILD_DLL -shared -o impl_b.dll impl_b.c

#линкуется на этапе компиляции с импортной libimpl_a.a
program1.exe: program1.c contract.h libimpl_a.a
	$(CC) -o program1.exe program1.c libimpl_a.a

#использует LoadLibrary, не требует импортной библиотеки
program2.exe: program2.c
	$(CC) -o program2.exe program2.c

clean:
	del /Q impl_a.dll impl_b.dll libimpl_a.a program1.exe program2.exe 2>nul || rm -f impl_a.dll impl_b.dll libimpl_a.a program1.exe program2.exe
