cmake_minimum_required (VERSION 3.8)

set(HEADER_LIB "PersistentVector.h" "PersistentMap.h" "Utils.h")

set(SOURCE_LIB "Utils.cpp")

add_library(PersistDataStructs STATIC ${HEADER_LIB} ${SOURCE_LIB})

add_executable(main main.cpp)

target_link_libraries(
  main
  PersistDataStructs
)
