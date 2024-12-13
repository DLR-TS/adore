find_package(PkgConfig REQUIRED)
pkg_check_modules(PROJ REQUIRED proj)

include_directories(${PROJ_INCLUDE_DIRS})
link_directories(${PROJ_LIBRARY_DIRS})


