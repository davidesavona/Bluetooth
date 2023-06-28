# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/bluetooth_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/bluetooth_autogen.dir/ParseCache.txt"
  "bluetooth_autogen"
  )
endif()
