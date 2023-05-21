cmake_minimum_required(VERSION 3.0.0)

file(GLOB GUI_INC_FILES
  "${PROJECT_SOURCE_DIR}/Assignment/*.h"
  "${PROJECT_SOURCE_DIR}/AssignmentDLL/*.h"
  )

file(GLOB GUI_SRC_FILES
  "${PROJECT_SOURCE_DIR}/Assignment/*.cpp"
)

message(STATUS "GUI_INC_FILES:" ${GUI_INC_FILES})
message(STATUS "GUI_SRC_FILES:" ${GUI_SRC_FILES})

if(EXECUTE_GUI)
  target_sources(Assignment PRIVATE ${GUI_SRC_FILES})
endif()
