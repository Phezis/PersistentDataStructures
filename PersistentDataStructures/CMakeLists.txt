cmake_minimum_required (VERSION 3.8)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB )

set(HEADER_LIB "PersistentVector.h")

add_library(PersistentDataStructures STATIC ${SOURCE_LIB} "PersistentList.h")

add_executable (main ${SOURCE_EXE} ${HEADER_LIB} "PersistentList.h")

target_link_libraries(main PersistentDataStructures)
