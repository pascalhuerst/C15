cmake_minimum_required(VERSION 3.2)

EXEC_PROGRAM("git"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ARGS "log -1 --pretty=format:\"%ad\" --date=iso"
  OUTPUT_VARIABLE PLAYGROUND_VERSION)

message("building C15 firmware version: ${PLAYGROUND_VERSION}")

add_subdirectory(playground)
add_subdirectory(bbbb)
add_subdirectory(NonMaps)
