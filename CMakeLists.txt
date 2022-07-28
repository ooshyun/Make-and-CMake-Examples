# Previous Trials
# cmake_minimum_required(VERSION 3.0.0)

# set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
# # set(CMAKE_OSX_ARCHITECTURES arm64)

# add_compile_options(-std=c11)
# add_compile_options(-fPIC -W -Wall -MD -MP -Wunused-variable)

# project(romdemo VERSION 0.1.0)
# message(${PROJECT_SOURCE_DIR})

# set(CURSES_NEED_NCURSES TRUE)
# find_package(Curses)
# message("Curses: ${CURSES_INCLUDE_DIRS}")

# set(HEADERS_SOURCE "${PROJECT_SOURCE_DIR}/lib")
# message("HEADERS_SOURCE: " ${HEADERS_SOURCE})

# set(SOURCES "${PROJECT_SOURCE_DIR}/src/main.c" 
#             "${PROJECT_SOURCE_DIR}/src/test_mtx.c" 
#             # "${PROJECT_SOURCE_DIR}/src/progressbar.c"
#             # "${PROJECT_SOURCE_DIR}/src/statusbar.c"
#     )

# message("--SOURCES--")
# message(${SOURCES})
# message("-----------")

# include_directories(
#   include
#   ${HEADERS_SOURCE}
#   ${LIBRARY}
#   ${CURSES_INCLUDE_DIRS}
# )

# message("Build type:" ${CMAKE_BUILD_TYPE})
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_COMPILER g++)

# set(OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/)
# message("Output:" ${OUTPUT_DIR})

# # Set output file type to exe 
# add_executable(${PROJECT_NAME} ${SOURCES})
# set_target_properties(${PROJECT_NAME} PROPERTIES EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})

project(Romdemo)

cmake_minimum_required(VERSION 3.0)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)
include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include/progressbar)

add_subdirectory(lib)
add_subdirectory(test)
