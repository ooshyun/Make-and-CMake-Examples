cmake_minimum_required(VERSION 3.0.0)

file(GLOB LIB_INC_FILES
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/*.h"
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/lib/*.h"
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/lib/*.hh"
  )

file(GLOB LIB_SRC_FILES
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/*.cpp"
)

message(STATUS "LIB_INC_FILES:" ${LIB_INC_FILES})
message(STATUS "LIB_SRC_FILES:" ${LIB_SRC_FILES})

if(LIBRARY)
  target_sources(CAssignment PRIVATE ${LIB_SRC_FILES})
endif()