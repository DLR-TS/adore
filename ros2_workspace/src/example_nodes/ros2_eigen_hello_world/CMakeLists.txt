cmake_minimum_required(VERSION 3.8)

get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(TOLOWER ${CURRENT_DIR_NAME} PROJECT)
project(${PROJECT})

include($ENV{SOURCE_DIRECTORY}/libraries/adore_libraries.cmake)


add_executable(${PROJECT} src/${PROJECT}.cpp)

include(requirements.cmake)



#ament_target_dependencies(ros2_eigen_hello_world rclcpp)

add_all_target_include_directories(${PROJECT})
add_all_target_link_libraries(${PROJECT})



install(TARGETS
    ${PROJECT}
  DESTINATION lib/${PROJECT}
)

ament_package()
