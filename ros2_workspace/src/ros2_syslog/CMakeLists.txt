cmake_minimum_required(VERSION 3.8)

get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(TOLOWER ${CURRENT_DIR_NAME} PROJECT)
project(${PROJECT})

include(requirements.cmake)

add_executable(${PROJECT} src/ros2_syslog.cpp)

include(../../../libraries/adore_libraries.cmake)
add_all_target_include_directories(${PROJECT})
add_all_target_link_libraries(${PROJECT})

install(TARGETS
  ros2_syslog
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
