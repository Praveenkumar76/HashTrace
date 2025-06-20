# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\plagiarism-detector_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\plagiarism-detector_autogen.dir\\ParseCache.txt"
  "plagiarism-detector_autogen"
  )
endif()
