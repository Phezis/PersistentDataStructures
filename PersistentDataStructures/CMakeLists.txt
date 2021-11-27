cmake_minimum_required (VERSION 3.8)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB PersistentDataStructures.cpp)

set(HEADER_LIB PersistentDataStructures.h)

add_library(PersistentDataStructures STATIC ${SOURCE_LIB})

add_executable (main ${SOURCE_EXE} ${HEADER_LIB})

target_link_libraries(main PersistentDataStructures)
