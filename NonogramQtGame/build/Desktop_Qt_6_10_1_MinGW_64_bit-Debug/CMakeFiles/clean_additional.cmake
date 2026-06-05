# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GameValidatorTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GameValidatorTests_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\NonogramGame_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NonogramGame_autogen.dir\\ParseCache.txt"
  "GameValidatorTests_autogen"
  "NonogramGame_autogen"
  )
endif()
