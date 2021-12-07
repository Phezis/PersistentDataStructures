cmake_minimum_required (VERSION 3.14)

project ("PersistentDataStructures")
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
IF (WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
ENDIF()

add_subdirectory ("PersistentDataStructures")
add_subdirectory ("PersistentDataStructuresTests")
