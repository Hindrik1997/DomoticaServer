#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Debug

#Toolchain
CC := E:/SysGCC/bin/arm-linux-gnueabihf-gcc.exe
CXX := E:/SysGCC/bin/arm-linux-gnueabihf-g++.exe
LD := $(CXX)
AR := E:/SysGCC/bin/arm-linux-gnueabihf-ar.exe
OBJCOPY := E:/SysGCC/bin/arm-linux-gnueabihf-objcopy.exe

#Additional flags
PREPROCESSOR_MACROS := DEBUG
INCLUDE_DIRS := 
LIBRARY_DIRS := 
LIBRARY_NAMES := pthread mysqlclient mysqlpp wiringPi
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O0 -std=c++14 -pthread -DMYSQLPP_MYSQL_HEADERS_BURIED
CXXFLAGS := -ggdb -ffunction-sections -O0 -std=c++14 -pthread -DMYSQLPP_MYSQL_HEADERS_BURIED
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections
COMMONFLAGS := 
LINKER_SCRIPT := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
CP_NOT_AVAILABLE := 1
IS_LINUX_PROJECT := 1
