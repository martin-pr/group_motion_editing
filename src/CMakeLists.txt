include_directories(./)

file(GLOB sources *.cpp)

add_executable(group_motion_editing ${sources})
target_link_libraries(group_motion_editing ${LIBS})
