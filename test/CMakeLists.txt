add_executable(romdemo main.c test_mtx.c)
target_link_libraries(romdemo progressbar statusbar ${CURSES_LIBRARIES})
