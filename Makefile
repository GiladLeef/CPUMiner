# Simplified Makefile for CPU Miner (Windows MSYS2)

# Compiler flags
CFLAGS = -O3 -march=native -Wall -Wextra -DUSE_ASM -DWIN32 \
	-Wno-unused-parameter -Wno-sign-compare -Wno-pointer-to-int-cast \
	-Wno-maybe-uninitialized -Wno-implicit-fallthrough -Wno-unused-variable

# Add flags to ignore Jansson library warnings (third-party code)
JANSSON_CFLAGS = $(CFLAGS) -Wno-stringop-overflow -Wno-format-truncation

INCLUDES = -I. -Icompat -Icompat/jansson

# Source files
C_SOURCES = cpu-miner.c util.c sha2.c
JANSSON_SOURCES = compat/jansson/dump.c compat/jansson/hashtable.c compat/jansson/load.c \
	compat/jansson/strbuffer.c compat/jansson/utf.c compat/jansson/value.c
ASM_SOURCES = sha2-x64.S

# Object files
C_OBJECTS = $(C_SOURCES:.c=.o)
JANSSON_OBJECTS = $(JANSSON_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.S=.o)
OBJECTS = $(C_OBJECTS) $(JANSSON_OBJECTS) $(ASM_OBJECTS)

# Target executable
TARGET = minerd.exe

# Config header
CONFIG_H = cpuminer-config.h

# Main target - build and clean up object files
all: build clean-obj

# Build target - create only the executable
build: $(CONFIG_H) $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) -L/mingw64/lib -lcurl -lpthread -lcrypto -lssl -lz -lws2_32
	strip -s $(TARGET)
	@echo Cleaning object files...
	@powershell -Command "Get-ChildItem -Path . -Filter '*.o' -Recurse | Remove-Item -Force"

# Generate config header
$(CONFIG_H):
	@echo "#define HAVE_STDINT_H 1" > $@
	@echo "#define HAVE_SYS_TYPES_H 1" >> $@
	@echo "#define HAVE_SYS_TIME_H 1" >> $@
	@echo "#define HAVE_UNISTD_H 1" >> $@
	@echo "#define HAVE_INTTYPES_H 1" >> $@
	@echo "#define WIN32 1" >> $@
	@echo "#define PACKAGE_NAME \"cpuminer\"" >> $@
	@echo "#define PACKAGE_VERSION \"2.5.1\"" >> $@
	@echo "#define PACKAGE_STRING \"cpuminer 2.5.1\"" >> $@
	@echo "#define STDC_HEADERS 1" >> $@
	@echo "#define USE_AVX 1" >> $@
	@echo "#define USE_AVX2 1" >> $@
	@echo "#define USE_XOP 1" >> $@
	@echo "#define HAVE_SHA256_4WAY 1" >> $@
	@echo "#define HAVE_SHA256_8WAY 1" >> $@

# Build rules for object files
%.o: %.c $(CONFIG_H)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Special rule for Jansson files to suppress specific warnings
compat/jansson/%.o: compat/jansson/%.c $(CONFIG_H)
	$(CC) $(JANSSON_CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.S $(CONFIG_H)
	$(CC) -x assembler-with-cpp $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean only object files, preserving executable
clean-obj:
	@echo Cleaning object files...
	@powershell -Command "Get-ChildItem -Path . -Filter '*.o' -Recurse | Remove-Item -Force"

# Clean all generated files including executable
clean:
	@echo Cleaning all files...
	@powershell -Command "Get-ChildItem -Path . -Filter '*.o' -Recurse | Remove-Item -Force"
	@powershell -Command "If (Test-Path '$(TARGET)') { Remove-Item -Force '$(TARGET)' }"
	@powershell -Command "If (Test-Path '$(CONFIG_H)') { Remove-Item -Force '$(CONFIG_H)' }"

.PHONY: all build clean clean-obj 