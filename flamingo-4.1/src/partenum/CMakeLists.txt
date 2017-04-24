cmake_minimum_required(VERSION 2.6)


set(VPARTENUM_LIB_SRC 
  src/vpartenum.cc
)


#set(VPARTENUM_EXAMPLE_EXEC_SRC
#  src/test.cc
#)

include_directories (
  .
  ./src/
  ${CMAKE_SOURCE_DIR}/../
  include
  lib
)

link_directories(
  ${CMAKE_SOURCE_DIR}/../util/build/
) 

add_subdirectory(../util/ ../util/)

add_definitions(-Wall -std=c++03  )
add_library(vpartenum-lib SHARED ${VPARTENUM_LIB_SRC})
target_link_libraries(vpartenum-lib ${CMAKE_REQUIRED_LIBRARIES})

#add_executable(example_vpartenum ${VPARTENUM_EXAMPLE_EXEC_SRC})
#add_dependencies(example_vpartenum vpartenum-lib util-lib)
#target_link_libraries(example_vpartenum vpartenum-lib util-lib rt)


set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/")

set(CMAKE_REQUIRED_LIBRARIES "vpartenum-lib;${CMAKE_REQUIRED_LIBRARIES}")
