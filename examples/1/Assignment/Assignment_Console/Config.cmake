cmake_minimum_required(VERSION 3.0.0)

file(GLOB EXE_INC_FILES
  "${PROJECT_SOURCE_DIR}/Assignment_Console/*.h"
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/*.h"
  )

file(GLOB EXE_SRC_FILES
  "${PROJECT_SOURCE_DIR}/Assignment_Console/*.cpp"
)

message(STATUS "EXE_INC_FILES:" ${EXE_INC_FILES})
message(STATUS "EXE_SRC_FILES:" ${EXE_SRC_FILES})

if(EXECUTE)
  target_sources(Assignment PRIVATE ${EXE_SRC_FILES})
endif()
