find_package(PkgConfig REQUIRED)
pkg_check_modules(PROJ REQUIRED proj)
find_package(Eigen3 REQUIRED)
set(Eigen3_TARGETS Eigen3::Eigen)

if(TARGET util)
    target_link_libraries(util PRIVATE ${PROJ_LIBRARIES})
endif()

