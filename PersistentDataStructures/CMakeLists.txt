cmake_minimum_required (VERSION 3.8)

set(SOURCE_EXE main.cpp)

set(HEADER_LIB "PersistentVector.h")

add_executable (main ${SOURCE_EXE} ${HEADER_LIB} "PersistentList.h" "Utils.h")
