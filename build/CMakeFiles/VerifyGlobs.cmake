# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.30
cmake_policy(SET CMP0009 NEW)

# bex_SRC at CMakeLists.txt:5 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "/home/rjajko/Code/Bex/*.cpp")
set(OLD_GLOB
  "/home/rjajko/Code/Bex/Bex.cpp"
  "/home/rjajko/Code/Bex/BexInterpreter.cpp"
  "/home/rjajko/Code/Bex/Scanner.cpp"
  "/home/rjajko/Code/Bex/Token.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rjajko/Code/Bex/build/CMakeFiles/cmake.verify_globs")
endif()

# bex_SRC at CMakeLists.txt:5 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "/home/rjajko/Code/Bex/*.h")
set(OLD_GLOB
  "/home/rjajko/Code/Bex/Bex.h"
  "/home/rjajko/Code/Bex/BexInterpreter.h"
  "/home/rjajko/Code/Bex/Scanner.h"
  "/home/rjajko/Code/Bex/Token.h"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rjajko/Code/Bex/build/CMakeFiles/cmake.verify_globs")
endif()
