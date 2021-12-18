cmake_minimum_required (VERSION 3.8)

set(HEADER_LIB "PersistentVector.h")

add_library(PersistDataStructs INTERFACE ${HEADER_LIB} "PersistentMap.h")

add_executable(main main.cpp "PersistentMap.h")
